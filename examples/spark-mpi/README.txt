The Spark-MPI demo that runs the MPI application on Spark workers

--------------------------------------------------------
On a single node
--------------------------------------------------------

1. Start the PMI server

export HYDRA_PROXY_PORT=55555

/opt/spark-mpi/bin/pmiserv -n 4 hello &

2. Submit allreduce.py

spark-submit ./allreduce.py

3. Stop the PMI proxy

pkill -9 "hydra_pmi_proxy"

--------------------------------------------------------
On a SLURM-based cluster
--------------------------------------------------------

1. Customize the sbatch script to your slurm installation

[edit allreduce.sh]

2. Submit the batch job

sbatch allreduce.sh
