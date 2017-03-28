#!/bin/bash                                                                                            

#SBATCH --partition [partition]
# your slurm may require accounting info...
# #SBATCH -A [account]
# ...or additional time and/or qos settings
# #SBATCH --qos [qos]
# #SBATCH --time=[hh:mm:ss] 

#SBATCH --nodes=2

start-master.sh

srun spark-class org.apache.spark.deploy.worker.Worker spark://$SLURMD_NODENAME:7077 &
spark-submit --master spark://$SLURMD_NODENAME:7077 ./collect.py 2

stop-master.sh
