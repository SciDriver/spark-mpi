The Spark-MPI demo that runs the MPI application on Spark workers

--------------------------------------------------------
On a single node
--------------------------------------------------------

1. Start the PMI server

pmixsrv -n 4 ./allreduce.py &

2. Submit allreduce.py

spark-submit ./allreduce.py

3. Stop the PMI server

pkill -9 "pmixsrv"

--------------------------------------------------------
On a SLURM-based cluster
--------------------------------------------------------

1. Customize the sbatch script to your slurm installation

[edit allreduce.sh]

2. Submit the batch job

sbatch allreduce.sh
