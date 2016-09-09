# import sys
# sys.argv = [2]
# python 2.7: execfile('./rdma-server.py')

import sparkmpi

size = sys.argv[0]
srv = sparkmpi.AddressServer.createServer()
addr = srv.start(size)
print (addr)


