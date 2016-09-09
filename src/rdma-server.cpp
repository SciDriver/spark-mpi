#include <iostream>
#include <sys/time.h>

#include "rdma_address.hpp"

using namespace std;

int main(int argc, char ** argv){

  if(argc != 2) {
    std::cout << "Usage: rdma-server <size> " << std::endl;
    return 0;
  }

  int size = atoi(argv[1]);

  RDMAAddressServer srv(size);

  std::cout << "server address: " << srv.address() << std::endl;

  while(1){
  }
 
  return 0;  
}
