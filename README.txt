
The project aims to extend the Spark platform with the MPI-oriented model
required by scientific-oriented applications. A general overview is provided 
in the NYSDS'16 paper located in the doc directory. 

1. Prerequisites
-----------------------------------------------

MPICH 3.2 

2. Installation (/opt/spark-mpi)
-----------------------------------------------

cmake ../spark-mpi -DBoost_NO_BOOST_CMAKE=ON
make
make install

3. Running applications
-----------------------------------------------

See the spark notebook in the examples directory


