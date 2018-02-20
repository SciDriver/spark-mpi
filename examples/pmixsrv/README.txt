The PMIServer-based demo that launches the MPI scripts without Spark.

--------------------------------------------------------
On a single node
--------------------------------------------------------

1. Make scripts executable

chmod a+x ./launcher.py ./allreduce.py

2. Start the PMI server

pmixsrv -n 4 ./allreduce.py &

3. Launch the MPI-based application

./launcher.py 

4. Stop the PMI server

pkill -9 "pmixsrv"

--------------------------------------------------------
On a SLURM-based cluster
--------------------------------------------------------

1. Customize the sbatch script to your slurm installation

[edit allreduce_pmiserv_sbatch.sh]

2. Submit the batch job

sbatch allreduce_pmiserv_sbatch.sh
