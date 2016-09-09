
The project aims to extend the Spark platform with the MPI-oriented model
required by scientific-oriented applications. A general overview is provided 
in the NYSDS'16 paper located in the doc directory. 

At this time, the project explores the CaffeOnSpark approach developed 
by Yahoo's team (https://github.com/yahoo/CaffeOnSpark). It added 
communication among the Spark workers based on the InfiniBand protocol. 
To facilitate analysis, this communication library was directly included 
into the source code, particularly, the src/caffe and src/util directories. 
Therefore, this application can only be used as a benchmark study, but 
we plan to generalize it with the MPI-based approach.   

1. Prerequisites
-------------------------------

SPARK(1.6.0; hadoop2.6), MVAPICH(2.1), CUDA(7.5)
Swig(3.0.8), Python(2.7), Boost(1.60), Glog

Because of constraints in the local cluster environment, Python and Glog
were installed under the user account. Therefore, their locations are
defined with the ANACONDA2_HOME and GLOG_HOME environmental variables. 


2. Building 
-------------------------------

cmake ../spark-mpi -DBoost_NO_BOOST_CMAKE=ON
make

3. Installing
-------------------------------

cmake ../spark-mpi -DBoost_NO_BOOST_CMAKE=ON \
    -DCMAKE_INSTALL_PREFIX=<spark-mpi installation directory>
make install

4. Running applications
-------------------------------

The examples/spark-mpi directory includes the benchmark application from 
the NYSDS'16 paper.


