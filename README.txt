
The project aims to extend the Spark platform
with the MPI-oriented model required by scientific
applications. A general overview is provided in
the NYSDS'16 paper located in the doc directory. 

-----------------------------------------------
1. Prerequisites
-----------------------------------------------

1.1 MPICH 3.2 or MVAPICH 2.2

./configure --disable-fortran --enable-cxx
make
make install

export MPI_SRC=<source directory>/src

1.2 mpi4py 2.0

pip install mpi4py

1.4 Spark 2.1

./build/mvn -DskipTests clean package

-----------------------------------------------
2. Installation (/opt/spark-mpi)
-----------------------------------------------

cmake -DBoost_NO_BOOST_CMAKE=ON
make
make install

-----------------------------------------------
3. Running applications
-----------------------------------------------

See the Jupyter notebook in the examples/spark directory


