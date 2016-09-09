#include <iostream>
#include <cstdlib>
#include <sys/time.h>
#include <mpi.h>

#include <cusp/array1d.h>
#include <cusp/blas.h>

using namespace std;

int main(int argc, char ** argv){

  int called = false;
  MPI_Initialized(&called);

  if(!called) {
    MPI_Init(&argc, &argv);
  }

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if(rank == 0){
    std::cout << "size: " << size << std::endl;
  }

  // gpu image

  int imageSize = 2*1000000*sizeof(float);
  int n = imageSize/sizeof(float);

  cusp::array1d<float, cusp::device_memory> x(n);

  for(int i = 0; i < 10; i++){

    // gpu-2-cpu converter (implemented after the SHARP Communicator API)

    struct timeval  start_tv;
    gettimeofday(&start_tv, NULL);

    cusp::array1d<float,cusp::host_memory> send(x);
    cusp::array1d<float,cusp::host_memory> recv(x.size());

    float* imageIn = (float*) send.data();
    imageIn[n-1] = 5.0*(i+1);

    struct timeval  conv1_tv;
    gettimeofday(&conv1_tv, NULL);

    // mpi

    MPI_Allreduce((float *) send.data(), (float* ) recv.data(), n, 
                   MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

    // cpu-2-gpu converter (implemented after the SHARP Communicator API)

    struct timeval  sum_tv;
    gettimeofday(&sum_tv, NULL);

    float* imageOut = (float*) recv.data();

    thrust::copy(recv.begin(), recv.end(), x.begin());

    struct timeval  conv2_tv;
    gettimeofday(&conv2_tv, NULL);

    if(rank == 0){

     printf ("mpi-demo, gpu-2-cpu, time(tv): = %f seconds \n",
	  (double) (conv1_tv.tv_usec - start_tv.tv_usec) / 1000000 +
	  (double) (conv1_tv.tv_sec - start_tv.tv_sec));

      printf ("mpi-demo, sum, time(tv): = %f seconds \n",
	  (double) (sum_tv.tv_usec - conv1_tv.tv_usec) / 1000000 +
	  (double) (sum_tv.tv_sec - conv1_tv.tv_sec));

      printf ("mpi-demo, cpu-2-gpu, time(tv): = %f seconds \n",
	  (double) (conv2_tv.tv_usec - sum_tv.tv_usec) / 1000000 +
	  (double) (conv2_tv.tv_sec - sum_tv.tv_sec));

      std::cout << "imageOut: " << imageOut[n-1] << std::endl;
    }
    
  }

  return 0;  
}
