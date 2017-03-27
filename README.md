# SPARK-MPI

The project addresses the existing impedance mismatch between data-intensive and compute-intensive ecosystems
by extending the [Spark](https://en.wikipedia.org/wiki/Apache_Spark) platform with
the [MPI](https://en.wikipedia.org/wiki/Message_Passing_Interface)-based inter-worker communication model
for supporting HPC applications. The rationale along with a general description are provided in
the Spark Summit East'17 talk and NYSDS'16 paper (located in
the [ doc ](https://github.com/SciDriver/spark-mpi/tree/master/doc)directory) :

* [Bringing HPC Algorithms to Big Data
Platforms](https://spark-summit.org/east-2017/events/bringing-hpc-algorithms-to-big-data-platforms/),
Spark Summit East, Boston, February 7-9, 2017
*  [Bringing HPC Reconstruction Algorithms to Big Data
Platforms](http://ieeexplore.ieee.org/document/7747818/),
Scientific Data Summit (NYSDS), New York, August 14-17, 2016

## Conceptual Demo

The Spark-MPI approach is illustrated within the context of a conceptual demo (located in the
[examples/spark](https://github.com/SciDriver/spark-mpi/tree/master/examples/spark )
directory) which runs the MPI Allreduce method on the Spark workers.

## Prerequisites

1. Binary and source code of the [MPICH 3.2](https://www.mpich.org/) implementation

```
cd <download directory>

./configure --disable-libxml2 --disable-fortran --disable-cxx --prefix=<installation directory>
make
sudo make install

export MPI_SRC=<download directory>/src
```
2. MPI Python wrapper, for example [mpi4py 2.0](http://pythonhosted.org/mpi4py/)
   ```
pip install mpi4py
```

3. Binary of the [Spark 2.1](http://spark.apache.org/) platform
   ```
cd <download directory>
./build/mvn -DskipTests clean package

export PYSPARK_DRIVER_PYTHON='jupyter'
export PYSPARK_DRIVER_PYTHON_OPTS='notebook'
```

## Installation 

```
git clone git://github.com/SciDriver/spark-mpi.git
mkdir build

cd build
cmake ../spark-mpi -DCMAKE_INSTALL_PREFIX=<installation directory>
make
sudo make install

```
