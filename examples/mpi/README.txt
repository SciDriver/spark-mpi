
qsub -I -l nodes=2:ppn=16:nvidia

cd <spark-mpi directory>/examples/mpi

cat $PBS_NODEFILE |sort |uniq >hostfile
mpirun -np 2 -f hostfile ./mpi-demo.bin
