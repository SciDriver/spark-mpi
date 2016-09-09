from __future__ import print_function

import sys
from datetime import timedelta, datetime, tzinfo
import numpy as np

from pyspark import SparkContext

import sparkmpi

sc = SparkContext(appName="SparkMPI")
print ("\nHello SparkMPI\n")

partitions = 2

srv = sparkmpi.AddressServer.createServer()
addr = srv.start(partitions)

print ("address: ", addr)

def f(args):

    comm = sparkmpi.Communicator.createCommunicator(args['rank'], 2)

    imageSize = 2*1000000
    comm.allocate(imageSize*4)

    comm.connect(args['addr'])

    a = np.zeros(imageSize, dtype=np.float32)
    a[imageSize-1] = 1.0

    t1 = datetime.now()
    for i in range(0, 10):
        comm.allSum(a)
    t2 = datetime.now()

    out = {
        'a' : a[imageSize-1],
        'time' : (t2-t1), 
    }
    
    comm.release()

    return out

inputs = []
for p in range(0, partitions):
    args = {
        'addr' : addr,
        'rank' : p,
    }
    inputs.append(args)

outs = sc.parallelize(inputs, partitions).map(f).collect()

for out in outs:
    print ("a: ", out['a'], ", processing time: ", out['time'])

sc.stop()
