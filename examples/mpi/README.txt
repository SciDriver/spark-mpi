The MPI-based demo that sums the float arrays of peers using
the MPI_Allreduce method.

--------------------------------------------------------
On a single node
--------------------------------------------------------

1. Make the script executable

chmod a+x ./allreduce.py

2. Launch the MPI applications

mpirun -np 4 ./allreduce.py

--------------------------------------------------------
On a SLURM-based cluster
--------------------------------------------------------

1. Customize the sbatch script to your slurm installation

[edit allreduce_sbatch.sh]

2. Submit the batch job

sbatch allreduce_sbatch.sh
