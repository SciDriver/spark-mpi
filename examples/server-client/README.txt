
1. start an address exchange server (for example, on node05)
for two workers

cd <spark-mpi>/examples/server-client

python

>>>import sys
>>>sys.argv = [2]
>>>execfile('./rdma-server.py')

This script starts an address exchange server and print its address,
for example, 

node05:38209

2. run the first worker (for example, on node06)

cd <spark-mpi>/examples/server-client

python

>>>import sys
>>>sys.argv = ["node05:38209", 0, 2]
>>>execfile('./rdma-worker.py')

3. run the second worker (for example, on node07)

cd <spark-mpi>/examples/server-client

python

>>>import sys
>>>sys.argv = ["node05:38209", 1, 2]
>>>execfile('./rdma-worker.py')





