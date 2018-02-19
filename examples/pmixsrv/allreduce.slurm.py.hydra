#!/usr/bin/env python

from __future__ import print_function

import os
from datetime import timedelta, datetime, tzinfo
import numpy as np

hostname = os.uname()[1]
hydra_proxy_port = os.getenv("HYDRA_PROXY_PORT")
pmi_port = hostname + ":" + hydra_proxy_port

slurm_nodeid = os.getenv("SLURM_NODEID")
slurm_nodes = os.getenv("SLURM_JOB_NUM_NODES")

# print ("pmi_port: ", pmi_port, ", pmi_id:", slurm_nodeid, "pmi_size:", slurm_nodes)

os.environ["PMI_PORT"] = pmi_port
os.environ["PMI_ID"]   = slurm_nodeid
os.environ["PMI_RANK"] = slurm_nodeid
os.environ["PMI_SIZE"] = slurm_nodes

from mpi4py import MPI

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

if rank == 0:
    print ("PMI_PORT: ", os.getenv("PMI_PORT"), ", rank: ", rank, ", size: ", size)

# image

n = 2*1000000
sendbuf = np.arange(n, dtype=np.float32)
recvbuf = np.arange(n, dtype=np.float32)

for i in range(10):

    sendbuf[n-1] = 5.0;

    t1 = datetime.now();

    comm.Allreduce(sendbuf, recvbuf, op=MPI.SUM)

    t2 = datetime.now()

    if rank == 0:
        print ("mpi-demo, sum: ", recvbuf[n-1], ", time:", (t2 - t1), " seconds")

MPI.Finalize()
