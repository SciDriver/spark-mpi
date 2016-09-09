

1. start the Spark master and two workers

1.1 start master (for example, on node05)

start-master.sh

check its address in the corresponding log file, for example:
spark://node05:7078

1.2 start workers (for example, on node06 and node07)

start-slave.sh -c 1 spark://node05:7078

2. run the spark-mpi script from node05

cd <spark-mpi>/examples/spark

spark-submit --master spark://node05:7078 spark-mpi.py 









