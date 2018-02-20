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

3. [Process Management Interface Exascale (PMIx)](https://github.com/pmix/pmix)

```
git clone https://github.com/pmix/pmix.git
./autogen.pl
./configure --prefix=<installation directory>
make
make install
```

4. [Open MPI](https://github.com/open-mpi/ompi) (with pmix:ext3x)

```
git clone https://github.com/open-mpi/ompi.git
./autogen.pl
./configure --prefix=<installation directory> --with-cuda --with-pmix=<pmix directory> --with-libevent=external
make
make install
```

5. MPI python wrapper, for example [mpi4py 3.0](http://mpi4py.readthedocs.io/en/stable/)

 ```
 wget https://bitbucket.org/mpi4py/mpi4py/downloads/mpi4py-3.0.0.tar.gz  
 python setup.py build
 python setup.py install
 ```

## Installation 

```
export MPI_SRC=<Open MPI source directory>

git clone git://github.com/SciDriver/spark-mpi.git
mkdir build

cd build
cmake ../spark-mpi -DCMAKE_INSTALL_PREFIX=<installation directory>
make
sudo make install

```
