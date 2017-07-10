from __future__ import print_function

import os
from datetime import timedelta, datetime, tzinfo
import numpy as np

from pyspark import SparkContext

sc = SparkContext()

# Define the address of the PMI server and the number of MPI workers

hostname = os.uname()[1]
hydra_proxy_port = os.getenv("HYDRA_PROXY_PORT")
pmi_port = hostname + ":" + hydra_proxy_port

partitions = 4

# Prepare a list of environmental variables

env = []
for id in range(0, partitions):
    kvs = {
        'PMI_PORT' : pmi_port,
        'PMI_ID' : id,
    }
    env.append(kvs)

# Create the rdd collection associated with the MPI workers

rdd = sc.parallelize(env, partitions)

# Define the MPI application

def allreduce(kvs):
    
     os.environ["PMI_PORT"] = kvs["PMI_PORT"]
     os.environ["PMI_ID"] = str(kvs["PMI_ID"])
    
     from mpi4py import MPI
    
     comm = MPI.COMM_WORLD
     rank = comm.Get_rank()
   
     # image

     n = 2*1000000
     sendbuf = np.arange(n, dtype=np.float32)
     recvbuf = np.arange(n, dtype=np.float32)
        
     sendbuf[n-1] = 5.0;

     t1 = datetime.now()    
     comm.Allreduce(sendbuf, recvbuf, op=MPI.SUM)     
     t2 = datetime.now()
    
     out = {
        'rank' : rank,
        'time' : (t2-t1), 
        'sum'  : recvbuf[n-1]
     }
     return out

# Run MPI application on Spark workers and collect the results

results = rdd.map(allreduce).collect()
for out in results:
    print ("rank: ", out['rank'], ", sum: ", out['sum'],
           ", processing time: ", out['time'])


