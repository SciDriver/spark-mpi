#include <iostream>
#include <arpa/inet.h>
#include <sys/time.h>

#include "RDMACommunicator.hpp"

using namespace std;

int main(int argc, char ** argv){

  if(argc != 4) {
    std::cout << "Usage: rdma-worker <address> <rank> <size> " << std::endl;
    return 0;
  }

  string srv_addr = argv[1];
  int rank = atoi(argv[2]);
  int size = atoi(argv[3]);

  RDMACommunicator rdmaComm(rank, size);

  int imageSize = 2*1000000*sizeof(float);
  rdmaComm.allocateImage(imageSize);

  rdmaComm.connectSrv(srv_addr);

  int n = imageSize/sizeof(float);
  float* image = (float*) malloc(imageSize);

  for(int i=0; i < 100; i++) {

    image[n-1] = 5.0;

    struct timeval  start_tv;
    gettimeofday(&start_tv, NULL);
  
    rdmaComm.allSum(image, n);

    struct timeval  sum_tv;
    gettimeofday(&sum_tv, NULL);

    printf ("rdma-worker, sum, time(tv): = %f seconds \n",
	  (double) (sum_tv.tv_usec - start_tv.tv_usec) / 1000000 +
	  (double) (sum_tv.tv_sec - start_tv.tv_sec));

    std::cout << "image[n-1]: " << image[n-1] << std::endl;
  }
  
  return 0;  
}
