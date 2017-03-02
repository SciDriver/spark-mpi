#!/bin/bash
#SBATCH --partition [partition]
# your slurm may require accounting info...
# #SBATCH -A [account]
# ...or additional time and/or qos settings
# #SBATCH --qos [qos]
# #SBATCH --time=[hh:mm:ss] 

# #SBATCH --nodes=4
# #SBATCH --ntasks-per-node=1
#SBATCH --ntasks=4

scontrol show hostname $SLURM_JOB_NODELIST | paste -d'\n' -s > hosts
mpirun -f hosts ./allreduce.py


