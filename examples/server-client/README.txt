This application is provided for debugging the spark-mpi approach
outside the Spark platform. In Spark, the corresponding scenario 
is coordinated by the Spark driver that starts the address exchange 
server and sums the float arrays on distributed workers. In this 
application, the exchange server and workers are run by scripts, 
rdma-server.py and rdma-worker.py.

1. start the address exchange server (for example, on node05)
for two workers (size = 2)

cd <spark-mpi>/examples/server-client

python

>>>import sys
>>>sys.argv = [2]
>>>execfile('./rdma-server.py')

This script starts the address exchange server and prints its address,
for example:

node05:38209

2. run the first worker (rank = 0, size = 2)

cd <spark-mpi>/examples/server-client

python

>>>import sys
>>>sys.argv = ["node05:38209", 0, 2]
>>>execfile('./rdma-worker.py')

3. run the second worker(rank = 1, size = 2)

cd <spark-mpi>/examples/server-client

python

>>>import sys
>>>sys.argv = ["node05:38209", 1, 2]
>>>execfile('./rdma-worker.py')





