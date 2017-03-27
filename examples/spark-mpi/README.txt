The Spark-MPI demo that runs the MPI application on Spark workers

--------------------------------------------------------
On a single node
--------------------------------------------------------

1. Define the Spark environmental variables for using
the Jupyter Notebook

export PYSPARK_DRIVER_PYTHON='jupyter'
export PYSPARK_DRIVER_PYTHON_OPTS='notebook'

2. Start the PMI server

/opt/spark-mpi/bin/pmiserv -n 4 hello

3. Run pyspark and edit pmi_port in allreduce.ipynb

pyspark

4. Stop the PMI proxy

pkill -9 "hydra_pmi_proxy"

--------------------------------------------------------
On a SLURM-based cluster
--------------------------------------------------------
