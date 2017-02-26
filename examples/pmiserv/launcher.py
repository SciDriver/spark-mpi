#!/usr/bin/env python

from __future__ import print_function

import os
import sys
from datetime import timedelta, datetime, tzinfo
import numpy as np

from multiprocessing import Process

def f(id, pmi_port):
    args = ['./allreduce.py']
    env = {}
    env["PATH"]     = os.getenv("PATH")
    env["PMI_ID"]   = str(id)
    env["PMI_PORT"] = pmi_port 
    os.execvpe('./allreduce.py', args, env)

if __name__ == '__main__':

   pmi_port = sys.argv[1]

   for id in range(4):
        p = Process(target=f, args=(id, pmi_port))
        p.start()
        
