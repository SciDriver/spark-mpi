The PMIServer-based demo that launches the MPI scripts without Spark:

chmod a+x ./launcher.py ./allreduce.py

/opt/spark-mpi/bin/pmiserv -n 2 hello

./launcher.py <PMI Port>


