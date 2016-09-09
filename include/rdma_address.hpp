#ifndef SHARP_SPARK_RDMA_ADDRESS_HPP_
#define SHARP_SPARK_RDMA_ADDRESS_HPP_

#include "util/socket.hpp"

class RDMAAddressServer : public caffe::SocketAdapter {

public:

  RDMAAddressServer(int size);

  virtual ~RDMAAddressServer();

  virtual void start_sockt_srvr();

  void send_addrs();

public:

  int counter;

};

class RDMAAddressClient  : public caffe::SocketChannel {

public:

  RDMAAddressClient(int rank);

public:

  virtual ~RDMAAddressClient();

  void send_addrs(std::string& rdma_addrs);

  void recv_addrs(std::string& rdma_addrs);

public:

  int rank;

};


#endif
