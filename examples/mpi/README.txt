The MPI-based demo that sums the float arrays of peers
using the MPI_Allreduce method.  

1. submit a batch job

qsub -I -l nodes=2:ppn=16:nvidia

2. run this application

cd <spark-mpi installation directory>/examples/mpi

cat $PBS_NODEFILE |sort |uniq >hostfile
mpirun -np 2 -f hostfile ./mpi-demo.bin
