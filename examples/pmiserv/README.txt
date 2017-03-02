The PMIServer-based demo that launches the MPI scripts without Spark.

--------------------------------------------------------
On a single node
--------------------------------------------------------

1. Make scripts executable

chmod a+x ./launcher.py ./allreduce.py

2. Start the PMI server

export HYDRA_PROXY_PORT=55555

/opt/spark-mpi/bin/pmiserv -n 4 hello

3. Launch the MPI-based application

./launcher.py <hostname>:HYDRA_PROXY_PORT

4. Stop the PMI proxy

pkill -9 "hydra_pmi_proxy"

--------------------------------------------------------
On a SLURM-based cluster
--------------------------------------------------------

1. Customize the sbatch script to your slurm installation

[edit allreduce_pmi_sbatch.sh]

2. Submit the batch job

sbatch allreduce_pmi_sbatch.sh
