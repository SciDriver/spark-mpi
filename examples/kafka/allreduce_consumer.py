from __future__ import print_function

import sys
import os
import numpy as np

from pyspark import SparkContext
from pyspark.streaming import StreamingContext
from pyspark.streaming.kafka import KafkaUtils, OffsetRange

def valueDecoder(v):
     import numpy
     import pickle
     recvbuf = pickle.loads(v)
     return recvbuf

def allreduce(iterator):

     import time
     from datetime import timedelta, datetime, tzinfo
     from pyspark import TaskContext

     # define the MPI enviromental variables

     pid = TaskContext.get().partitionId();

     hostname = os.uname()[1]
     hydra_proxy_port = os.getenv("HYDRA_PROXY_PORT")
     pmi_port = hostname + ":" + hydra_proxy_port
    
     os.environ["PMI_PORT"] = pmi_port
     os.environ["PMI_ID"]   = str(pid)

     # initialize the MPI inter-worker environment

     t1 = datetime.now();
    
     from mpi4py import MPI   
     comm = MPI.COMM_WORLD
     rank = comm.Get_rank()

     t2 = datetime.now()
     print ("pid: ", pid, "time: ", (t2 - t1), " seconds", "rank: ", rank)

     # run the MPI inter-worker commands  

     it = 0 
     for x in iterator:
          n = len(x[1])
          recvbuf = np.arange(n, dtype=np.float32)
          comm.Allreduce(x[1], recvbuf, op=MPI.SUM)
          print("pid: ", pid, "it: ", it, n, recvbuf[n-1])
          it += 1

     yield pid

if __name__ == "__main__":
    
     if len(sys.argv) != 2:
          print("Usage: allreduce_consumer.py <zk>", file=sys.stderr)
          exit(-1)

     zkQuorum = sys.argv[1]

     partitions = 4 # spark partitions

     sc = SparkContext(appName="KafkaSparkMPI")

     # create the Kafka RDDs

     kafkaParams = {"metadata.broker.list": "localhost:9092"}
     start = 0
     until = 4
     partition = 0 # kafka partition

     kafkaRDDs = []
     for j in range(partitions):
          topic = "topic-" + str(j)
          offset  = OffsetRange(topic, partition, start, until)
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

 

 
     

  
