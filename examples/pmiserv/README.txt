The PMIServer-based demo that launches the MPI scripts without Spark.

--------------------------------------------------------
On a single node
--------------------------------------------------------

1. Make scripts executable

chmod a+x ./launcher.py ./allreduce.py

2. Start the PMI server

/opt/spark-mpi/bin/pmiserv -n 4 hello

3. Lauch the MPI-based application

./launcher.py <PMI Port>

4. Stop the PMI proxy

pkill -9 "hydra_pmi_proxy"

--------------------------------------------------------
On a SLURM-based cluster
--------------------------------------------------------





