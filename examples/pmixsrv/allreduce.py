#!/usr/bin/env python

from __future__ import print_function

import os
from datetime import timedelta, datetime, tzinfo
import numpy as np

print("PMIX_RANK: ", os.getenv("PMIX_RANK"))
print("OMPI_MCA_orte_local_daemon_uri: ", os.getenv("OMPI_MCA_orte_local_daemon_uri"))
print("OMPI_MCA_orte_hnp_uri: ", os.getenv("OMPI_MCA_orte_hnp_uri"))
print("PMIX_NAMESPACE: ", os.getenv("PMIX_NAMESPACE"))
print("PMIX_SERVER_URI2: ", os.getenv("PMIX_SERVER_URI2"))
print("PMIX_SERVER_URI21:", os.getenv("PMIX_SERVER_URI21"))

from mpi4py import MPI

comm = MPI.COMM_WORLD
rank = comm.Get_rank()

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
