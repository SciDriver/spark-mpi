MPI-oriented extension of the Spark model

cmake ../spark-mpi -DBoost_NO_BOOST_CMAKE=ON

make

cmake ../spark-mpi -DBoost_NO_BOOST_CMAKE=ON -DCMAKE_INSTALL_PREFIX=/home/malitsky/opt/spark-mpi 

make install

