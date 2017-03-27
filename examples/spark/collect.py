from __future__ import print_function

import os
import sys
from datetime import timedelta, datetime, tzinfo
import numpy as np

from pyspark import SparkContext

sc = SparkContext()

partitions = int(sys.argv[1]) if len(sys.argv) > 1 else 1

# Prepare a list of arguments

n = 2*1000000

env = []
for id in range(0, partitions):
    kvs = {
        'n' : n,
        'rank' : id,
    }
    env.append(kvs)

# Create the RDD instance

rdd = sc.parallelize(env, partitions)

def sendbuf(kvs):

     rank = kvs['rank']
     n = kvs['n']
      
     # image

     sendbuf = np.arange(n, dtype=np.float32)      
     sendbuf[n-1] = 5.0;

     t1 = datetime.now()    
    
     out = {
        'rank' : rank,
        't1' : t1, 
        'buffer'  : sendbuf
     }
     return out

# Collect buffers from distributed workers

results = rdd.map(sendbuf).collect()

# Print timing

t2 = datetime.now()

for out in results:
    print ("rank: ", out['rank'], ", buffer: ", out['buffer'][n-1], ", processing time: ", t2 - out['t1'])

