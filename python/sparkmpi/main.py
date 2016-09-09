import sys
import sparkmpi

def startAddressServer(size):
    srv = sparkmpi.PythonRDMAAddressServer(size)
    return srv.address()


