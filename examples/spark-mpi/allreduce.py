from __future__ import print_function

import os
from datetime import timedelta, datetime, tzinfo
import numpy as np

from pyspark import SparkContext, TaskContext

sc = SparkContext()

# Define the number of MPI workers

partitions = 4

# Read the PMIx environmental variables

env = {}
with open('pmixsrv.env', 'r') as f:
    lines = f.read().splitlines() 
    for line in lines:
        words = line.split("=")
        env[words[0]] = words[1]

# Create the rdd collection associated with the MPI workers

arg = []
for id in range(partitions):
    arg.append(env)
    
rdd = sc.parallelize(arg, partitions)

# Define the MPI application

def allreduce(pid, partition):

    os.environ["PMIX_RANK"] = str(pid)

    for env in partition:
        for key in env:
            print(key, env[key])
            os.environ[key] = env[key]
    
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
    yield out

# Run MPI application on Spark workers and collect the results

results = rdd.mapPartitionsWithIndex(allreduce).collect()
print ("1st run")
for out in results:
    print ("rank: ", out['rank'], ", sum: ", out['sum'],
           ", processing time: ", out['time'])



