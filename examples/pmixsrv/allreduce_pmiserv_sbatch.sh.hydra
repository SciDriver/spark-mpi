#!/bin/bash
#SBATCH --partition [partition]
# your slurm may require accounting info...
# #SBATCH -A [account]
# ...or additional time and/or qos settings
# #SBATCH --qos [qos]
# #SBATCH --time=[hh:mm:ss] 

#SBATCH --nodes=4
#SBATCH --ntasks-per-node=1

module load spark-mpi/0.1                                                                          
scontrol show hostname $SLURM_JOB_NODELIST | paste -d'\n' -s > hosts

# Start the PMI server
export HYDRA_PROXY_PORT=55555
pmiserv -f hosts hello &

# Launch the MPI-based application
srun ./allreduce.slurm.py

# Stop the PMI proxy
srun pkill "hydra_pmi_proxy"

