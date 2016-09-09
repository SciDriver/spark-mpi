
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${INCLUDE_LIST})

LIST(APPEND LIBRARY_LIST "/usr/lib64/libibverbs.so")

# set (CMAKE_CXX_FLAGS "-D CPU_ONLY")
set (CMAKE_CXX_FLAGS "-D INFINIBAND")

message("all libraries: ${LIBRARY_LIST}")

set (spark_yahoo_src
    ./caffe/common.cpp
    ./caffe/internal_thread.cpp
    ./util/rdma.cpp
    ./util/blocking_queue_extra.cpp
    ./rdma_address.cpp
    ./RDMACommunicator.cpp
)

CUDA_ADD_LIBRARY(spark-mpi SHARED ${spark_yahoo_src})
TARGET_LINK_LIBRARIES(spark-mpi ${LIBRARY_LIST})
INSTALL(TARGETS spark-mpi LIBRARY DESTINATION lib)

# CUDA_ADD_EXECUTABLE(mpi-demo.bin mpi-demo.cu)
# TARGET_LINK_LIBRARIES(mpi-demo.bin spark-mpi)
# INSTALL(TARGETS mpi-demo.bin RUNTIME DESTINATION bin)

# ADD_EXECUTABLE(rdma-server.bin rdma-server.cpp)
# TARGET_LINK_LIBRARIES(rdma-server.bin spark-mpi)
# INSTALL(TARGETS rdma-server.bin RUNTIME DESTINATION bin)

# ADD_EXECUTABLE(rdma-worker.bin rdma-worker.cpp)
# TARGET_LINK_LIBRARIES(rdma-worker.bin spark-mpi)
# INSTALL(TARGETS rdma-worker.bin RUNTIME DESTINATION bin)


