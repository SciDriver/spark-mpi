#!/usr/bin/env python

from __future__ import print_function

import os
from datetime import timedelta, datetime, tzinfo
import numpy as np

from mpi4py import MPI

pmi_rank = os.getenv("PMI_RANK")
pmi_fd   = os.getenv("PMI_FD")
pmi_size = os.getenv("PMI_SIZE")

pmi_port = os.getenv("PMI_PORT")
pmi_id = os.getenv("PMI_ID")

comm = MPI.COMM_WORLD
rank = comm.Get_rank()

if rank == 0:
     print ("PMI_RANK: ", pmi_rank, ", PMI_SIZE: ", pmi_size, "PMI_FD: ", pmi_fd)
     print ("PMI_PORT: ", pmi_port, ", PMI_ID: ", pmi_id)

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

