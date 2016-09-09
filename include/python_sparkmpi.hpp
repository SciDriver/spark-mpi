#ifndef PYTHON_SPARK_MPI_H
#define PYTHON_SPARK_MPI_H

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION

#include <Python.h>
#include <string>
#include <tr1/memory>
#include <numpy/arrayobject.h>

#include "rdma_address.hpp"

class RDMACommunicator;

namespace sparkmpi {

class AddressServer;
typedef std::tr1::shared_ptr<AddressServer> AddressServerPtr;

class Communicator;
typedef std::tr1::shared_ptr<Communicator> CommunicatorPtr;

class AddressServer {

public:

  static AddressServerPtr createServer();

  ~AddressServer() {}

public:

  std::string start(int size);

  void release() {}

private:

  AddressServer();

private:

  std::tr1::shared_ptr<RDMAAddressServer> adaptee;

};

class Communicator {

public:

  static CommunicatorPtr createCommunicator(int rank, int size);

  ~Communicator() {}

public:

  void allocate(int size);

  void connect(char* srv_addr);

  void release();

public:

  void allSum(PyObject* nparray);

private:

  Communicator(int rank, int size);

private:

  std::tr1::shared_ptr<RDMACommunicator> adaptee;

};


}

#endif
