The PMIServer-based demo that launches the MPI scripts without Spark:

export PATH=/opt/spark-mpi/bin:$PATH
which hydra_pmi_proxy

chmod a+x ./launcher.py ./allreduce.py

pmiserv -n 2 hello

./launcher.py <PMI Port>


