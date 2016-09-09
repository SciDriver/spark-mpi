#define NO_IMPORT_ARRAY
#define PY_ARRAY_UNIQUE_SYMBOL sparkmpi_ARRAY_API

#include "python_sparkmpi.hpp"
#include <iostream>

#define INFINIBAND
#include "RDMACommunicator.hpp"

namespace sparkmpi {

AddressServer::AddressServer(){
}

AddressServerPtr AddressServer::createServer(){
  AddressServerPtr s(new AddressServer());
  return s;
}

std::string AddressServer::start(int size){
  adaptee.reset(new RDMAAddressServer(size));
  return adaptee->address();
}

Communicator::Communicator(int rank, int size){
  adaptee.reset(new RDMACommunicator(rank, size));
}

CommunicatorPtr Communicator::createCommunicator(int rank, int size){
    CommunicatorPtr s(new Communicator(rank, size));
  return s;
}

void Communicator::allocate(int size){
  adaptee->allocateImage(size);
}

void Communicator::connect(char* addr){
  string srv_addr = addr;
  adaptee->connectSrv(srv_addr);
}

void Communicator::release(){
  adaptee->release();
}

//

void Communicator::allSum(PyObject* obj){

  PyArrayObject* npArray = 
    (PyArrayObject*) PyArray_FROM_OTF(obj, NPY_FLOAT32, NPY_ARRAY_INOUT_ARRAY);

  if(npArray == 0) {
    std::cout << "allSum, npArray == 0" << std::endl;
    return;
  }
  
  // int nd = PyArray_NDIM(npArray);
  // std::cout << "allSum, dim: " << nd << std::endl;

  // PyArray_Descr* descr =  PyArray_DESCR(npArray);

  float* data = (float*) PyArray_DATA(npArray);
  int n = PyArray_DIMS(npArray)[0];

  adaptee->allSum(data, n);

  Py_DECREF(npArray);  
}



}
