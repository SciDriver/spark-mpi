
Project aims to extend the Spark platform with the MPI-oriented model
required by scientific-oriented applications. 

At this time, it explores the CaffeOnSpark approach developed by Yahoo's team
(https://github.com/yahoo/CaffeOnSpark) adding a peer-to-peer communication
based on the InfiniBand protocol. To facilitate analysis, this communication
library was directly included into the source code, particularly, 
the src/caffe and src/util directories. Therefore, this application can be 
only used for the benchmark study and we plan to generalize it with 
the MPI-based approach.   

1. Prerequisites
-------------------------------



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

