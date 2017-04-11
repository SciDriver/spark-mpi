from __future__ import print_function

import os
import sys
import socket
from datetime import timedelta, datetime, tzinfo
import numpy as np

from pyspark import SparkContext

sc = SparkContext()

# Define the address of the PMI server and the number of MPI workers

partitions = int(sys.argv[1]) if len(sys.argv) > 1 else 1

# Create the rdd collection associated with the MPI workers

env = [id for id in range(partitions)]
rdd = sc.parallelize(env, partitions)

# Define the MPI application

def allreduce(kvs):
   
    hostname = socket.gethostname()
    hydra_proxy_port = os.getenv("HYDRA_PROXY_PORT")
    pmi_port = hostname + ":" + hydra_proxy_port

    slurm_nodeid = os.getenv("SLURM_NODEID")

    os.environ["PMI_PORT"] = pmi_port
    os.environ["PMI_ID"]   = slurm_nodeid

    # unsetenv the SLURM variables

    # del os.environ["PMI_RANK"]
    # del os.environ["PMI_SIZE"]
    del os.environ["PMI_FD"] 
    
    from mpi4py import MPI
    
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
   
    # image

    n = 2*1000000
    sendbuf = np.arange(n, dtype=np.float32)
    recvbuf = np.arange(n, dtype=np.float32)

    t1 = datetime.now()    

    for i in range(10):
        
        sendbuf[n-1] = i;
        comm.Allreduce(sendbuf, recvbuf, op=MPI.SUM)    
 
    t2 = datetime.now()
    
    out = {
        'rank' : rank,
        'time' : (t2-t1)/10, 
        'sum'  : recvbuf[n-1]
    }
    return out

# Run MPI application on Spark workers and collect the results

results = rdd.map(allreduce).collect()
for out in results:
    print ("rank: ", out['rank'], ", sum: ", out['sum'],
           ", processing time: ", out['time'])


