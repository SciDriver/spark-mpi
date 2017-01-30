The Spark-MPI demo that runs the MPI application on Spark workers

1. Start the PMI server

/opt/spark-mpi/bin/pmiserv -n 4 hello

2. Define the Spark environmental variables for using
the Jupyter Notebook

export PYSPARK_DRIVER_PYTHON='jupyter'
export PYSPARK_DRIVER_PYTHON_OPTS='notebook'

3. Run pyspark and edit pmi_port in allreduce.ipynb
