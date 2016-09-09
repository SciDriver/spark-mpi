# import sys
# sys.argv = [addr, rank, size]
# python 2.7: execfile('./rdma-worker.py')

from datetime import timedelta, datetime, tzinfo
import numpy as np
import sparkmpi

addr = sys.argv[0]
rank = sys.argv[1]
size = sys.argv[2]
comm = sparkmpi.Communicator.createCommunicator(rank, size)

imageSize = 2*1000000
comm.allocate(imageSize*4)

comm.connect(addr)

a = np.zeros(imageSize, dtype=np.float32)
a[imageSize-1] = 5.0

t1 = datetime.now()
comm.allSum(a)
t2 = datetime.now()

print "processing time: ", (t2 - t1);
print "a[n-1]: ", a[imageSize-1]

comm.release()




