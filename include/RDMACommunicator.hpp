#ifndef SHARP_SPARK_RDMA_COMMUNICATOR_HPP_
#define SHARP_SPARK_RDMA_COMMUNICATOR_HPP_

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "util/rdma.hpp"

#include "rdma_address.hpp"

class RDMACommunicator {

public:

  RDMACommunicator(int rank, int size);

  virtual ~RDMACommunicator();

public: 

  void allocateImage(int size);

  uint8_t* getImage();

  bool connectSrv(std::string& srv_addr);

  void release();

public:

  // Communicator API

  virtual void allSum(float * v, int n);

protected:

  // client for communicating with the address server

  RDMAAddressClient addr_client_;

  // memory buffer with all peers' images

  size_t size_;
  uint8_t* data_;

protected:

  // CaffeNet API

  /**
   * retrieve the server address in which we will accept messages from peers in the cluster
   *
   * @return a collection of server addresses
   */
  void localAddresses(std::vector<std::string>& vec);

  /**
   * establish connection with cluster peers
   *
   * @param addresses Array of addresses, whose index represents rank
   * @return true if connected successfully
   */
  bool connect(std::vector<std::string >& addresses); 

  std::string address(){
     return adapter_->name();
  }

  void sync();

protected:

  // RDMASync methods

  void chunk(int peer, size_t* offs, size_t* size);

  void CreateMasterBuffers(int peer);
  void CreateWorkerBuffers(int peer);

  // RDMASync members

  int cluster_size_, node_rank_;

  boost::shared_ptr<caffe::RDMAAdapter> adapter_;
  std::vector<boost::shared_ptr<caffe::RDMAChannel> > peers_;

  // Each node is parameter server for a shard, defined as an offset and size
  size_t own_offs_;
  size_t own_size_;

  // RDMA mappings on weights and gradients buffers, allow send and receive
  std::vector<boost::shared_ptr<caffe::RDMABuffer> > data_send_;
  std::vector<boost::shared_ptr<caffe::RDMABuffer> > data_recv_;

};

#endif
