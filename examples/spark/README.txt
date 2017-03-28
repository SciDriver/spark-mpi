The Spark benchmark application that collects buffers from
distributed workers. 

--------------------------------------------------------
On a single node
--------------------------------------------------------

spark-submit ./collect.py <partitions>

--------------------------------------------------------
On a SLURM-based cluster
--------------------------------------------------------

1. Customize the sbatch script to your slurm installation

[edit collect_sbatch.sh]

2. Submit the batch job

sbatch collect_sbatch.sh
