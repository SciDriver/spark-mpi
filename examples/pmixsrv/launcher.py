#!/usr/bin/env python

from __future__ import print_function

import os
import sys
from datetime import timedelta, datetime, tzinfo
import numpy as np

from multiprocessing import Process

def f(id):
    args = ['./allreduce.py']
    
    env = {}
    env["PATH"] = os.getenv("PATH")
    env["PMIX_RANK"]  = str(id)

    with open('pmixsrv.env', 'r') as f:
        lines = f.read().splitlines() 
        for line in lines:
            words = line.split("=")
            env[words[0]] = words[1]

    os.execvpe('./allreduce.py', args, env)

if __name__ == '__main__':

   # pmi_port = sys.argv[1]

   for id in range(4):
        p = Process(target=f, args=(id,))
        p.start()
        
