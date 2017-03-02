#!/bin/bash
#SBATCH --partition [partition]
# your slurm may require accounting info...
# #SBATCH -A [account]
# ...or additional time and/or qos settings
# #SBATCH --qos [qos]
# #SBATCH --time=[hh:mm:ss] 

#SBATCH --nodes=4
#SBATCH --ntasks-per-node=1
# #SBATCH --ntasks=4

scontrol show hostname $SLURM_JOB_NODELIST | paste -d'\n' -s > hosts
HOSTS=$(scontrol show hostname $SLURM_JOB_NODELIST | paste -d, -s)

### WARNING, Work In Progress!!! incomplete?!?  untested!!!

# Start the PMI server
export HYDRA_PROXY_PORT=55555
/opt/spark-mpi/bin/pmiserv -f hosts hello

# I'm not sure we need '-w $HOSTS' on these?!

# Launch the MPI-based application
srun -w $HOSTS ./allreduce.py

# Stop the PMI proxy
srun -w $HOSTS pkill -9 "hydra_pmi_proxy"

