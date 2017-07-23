from __future__ import print_function

import sys
import os
import time
from datetime import timedelta, datetime, tzinfo
import numpy as np

from kafka import KafkaConsumer

from pyspark import SparkContext
from pyspark.streaming import StreamingContext
from pyspark.streaming.kafka import KafkaUtils, OffsetRange

def valueDecoder(v):
     import numpy
     import pickle
     recvbuf = pickle.loads(v)
     return recvbuf

def allreduce(iterator):

     def inner_mpi(pid, iterator):
          
          print("hello", pid)

          import time
          from datetime import timedelta, datetime, tzinfo

          # define the MPI enviromental variables

          hostname = os.uname()[1]
          hydra_proxy_port = os.getenv("HYDRA_PROXY_PORT")
          pmi_port = hostname + ":" + hydra_proxy_port
          os.environ["PMI_PORT"] = pmi_port  
          os.environ["PMI_ID"]   = str(pid)

          # initialize the MPI inter-worker environment
    
          from mpi4py import MPI
          comm = MPI.COMM_WORLD
          rank = comm.Get_rank()

          # run the MPI inter-worker commands  

          it = 0 
          for x in iterator:
               n = len(x[1])
               recvbuf = np.arange(n, dtype=np.float32)
               comm.Allreduce(x[1], recvbuf, op=MPI.SUM)
               print("pid: ", pid, "it: ", it, n, recvbuf[n-1])
               it += 1

          time.sleep(30)
          
     from pyspark import TaskContext
     from multiprocessing import Process

     pid = TaskContext.get().partitionId();

     t1 = datetime.now(); 
     p = Process(target=inner_mpi, args = (pid, iterator, ))
     p.start()
     p.join()
     t2 = datetime.now()
     print ("pid: ", pid, "time: ", (t2 - t1), " seconds")

     yield pid

def run_batch(sc, partitions, start, until):

     # create the Kafka RDDs

     kafkaParams = {"metadata.broker.list": "localhost:9092"}

     kafkaRDDs = []
     for j in range(partitions):
          topic = "topic-" + str(j)
          offset  = OffsetRange(topic, 0, start, until)
          offsets = [offset]
          topicRDD = KafkaUtils.createRDD(sc, kafkaParams,offsets,
                                          valueDecoder=valueDecoder)
          kafkaRDDs.append(topicRDD)

     # combine different topics
     
     unionRDD = sc.union(kafkaRDDs)
     numParts = unionRDD.getNumPartitions()
     print("number of partitions: ", numParts)

     # run the MPI application
     unionRDD.mapPartitions(allreduce).count()


if __name__ == "__main__":

     partitions = 4 # spark partitions

     sc = SparkContext(appName="KafkaSparkMPI")

     # Check a new cycle
     consumer = KafkaConsumer('topic-init',
                              group_id='my-group',
                              bootstrap_servers=['localhost:9092'],
                              auto_offset_reset='latest',
                              enable_auto_commit=True)

     for message in consumer:

          consumer.commit()
          print("%s:%d:%d: key=%s value=%s" % (message.topic, message.partition,
                                               message.offset, message.key,
                                               message.value))
 
          if os.system('/opt/spark-mpi/bin/pmiserv -n 4 hello &') != 0:
               print ("pmiserv: ERROR")
               break  
          time.sleep(2)

          run_batch(sc, partitions, 0, 4)

          if os.system("pkill -9 \"hydra_pmi_proxy\" &") != 0:
               print ("pkill: ERROR")
               break
          time.sleep(2)

          print("cycle is done")


 
 

 
     

  
