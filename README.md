# SPARK-MPI

The project addresses the existing impedance mismatch between data-intensive and compute-intensive ecosystems
by extending the [Spark](https://en.wikipedia.org/wiki/Apache_Spark) platform with
the [MPI](https://en.wikipedia.org/wiki/Message_Passing_Interface)-based inter-worker communication model
for supporting HPC applications. The rationale along with a general description are provided in the
NYSDS papers and the Spark Summit East'17 talk (located in
the [ doc ](https://github.com/SciDriver/spark-mpi/tree/master/doc)directory) :

* [Building Near-Real-Time Processing Pipelines with the Spark-MPI platform](http://ieeexplore.ieee.org/document/8085039/), NYSDS, New York, August 7-9, 2017
* [Bringing HPC Algorithms to Big Data
Platforms](https://databricks.com/session/bringing-hpc-algorithms-to-big-data-platforms),
Spark Summit East, Boston, February 7-9, 2017
*  [Bringing HPC Reconstruction Algorithms to Big Data
Platforms](http://ieeexplore.ieee.org/document/7747818/), NYSDS, New York, August 14-17, 2016

## Conceptual Demo

The Spark-MPI approach is illustrated within the context of a conceptual demo (located in the
[examples/spark-mpi](https://github.com/SciDriver/spark-mpi/tree/master/examples/spark-mpi )
directory) which runs the MPI Allreduce method on the Spark workers.

## Prerequisites

1. Python 3.5, for example  [Anaconda3-4.2.0](https://www.continuum.io) (note: Spark 2.1 does not support Python 3.6)

2. [Spark 2.2](https://spark.apache.org/downloads.html) 

```
download spark
cd spark
export PYSPARK_PYTHON=python3
./build/mvn -DskipTests clean package
```

3. [MVAPICH2 2.2](http://mvapich.cse.ohio-state.edu/) and associated python module

   3.1 Binary and source code of the MVAPICH2 2.2 implementation

   ```
   cd <download directory>

   ./configure --disable-libxml2 --disable-fortran --prefix=<installation directory>
   make
   sudo make install
   ```

   3.2  MPI python wrapper, for example [mpi4py 2.0](http://pythonhosted.org/mpi4py/)

   ```
   pip install mpi4py
   ```

## Installation 

```
export MPI_SRC=<MVAPICH2 directory>/src

git clone git://github.com/SciDriver/spark-mpi.git
mkdir build

cd build
cmake ../spark-mpi -DCMAKE_INSTALL_PREFIX=<installation directory>
make
sudo make install

```
