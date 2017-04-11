#!/bin/bash
#SBATCH --partition [partition]
# your slurm may require accounting info...
# #SBATCH -A [account]
# ...or additional time and/or qos settings
# #SBATCH --qos [qos]
# #SBATCH --time=[hh:mm:ss]

#SBATCH --nodes=10

module load spark-mpi/0.1

export HYDRA_PROXY_PORT=55555
scontrol show hostname $SLURM_JOB_NODELIST | paste -d'\n' -s > hosts10
pmiserv  -f hosts10 hello &

export SPARK_WORKER_CORES=1
start-master.sh
srun spark-class org.apache.spark.deploy.worker.Worker spark://$SLURMD_NODENAME:7077 &

spark-submit  --master spark://$SLURMD_NODENAME:7077 ./allreduce.py 10

stop-master.sh
