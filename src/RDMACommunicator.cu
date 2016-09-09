#include "RDMACommunicator.hpp"

using namespace std;
using namespace caffe;

RDMACommunicator::RDMACommunicator(int rank, int size) {

  node_rank_ = rank;
  cluster_size_ = size;
    
  adapter_.reset(new RDMAAdapter());
  LOG(INFO)<< "RDMA adapter: " << adapter_->name();

  // The node creates a RDMA address for each node in the cluster except itself.
  // The RDMA addresses are ordered according to the rank of the peers.
  // Create channel for each peer
  peers_.resize(this->cluster_size_);
  for (int i = 0; i < this->cluster_size_; i++) {
    if (i != this->node_rank_)
      peers_[i].reset(new RDMAChannel(*adapter_));
  }

  data_send_.resize(this->cluster_size_);
  data_recv_.resize(this->cluster_size_);

}

RDMACommunicator::~RDMACommunicator() {

  for (int i=0; i<this->cluster_size_; i++){
    peers_[i].reset();
  }
  adapter_.reset();

  CUDA_CHECK(cudaFree(data_));
}

void RDMACommunicator::allocateImage(int size){
  size_ = size*peers_.size();
  CUDA_CHECK(cudaMalloc(&data_, size));
}

uint8_t* RDMACommunicator::getImage(){
  return (data_ + own_offs_);
}

void RDMACommunicator::localAddresses(vector<string>& vec) {
    vec.resize(this->cluster_size_);
    for (int i = 0; i < this->cluster_size_; i++) {
        if (i != this->node_rank_) {
            vec[i] = peers_[i]->address();
        } else {
            vec[i] = "";
	}
        LOG(INFO) << i << "-th RDMA addr: " << vec[i].c_str();
    }
}

bool RDMACommunicator::connect(vector<const char*>& peer_addresses) {

  //establish RDMA connections
  for (int i = 0; i < this->cluster_size_; i++){
      std::cout << i << std::endl;
        if (i != this->node_rank_) {
	    std::cout << "address: " << peer_addresses[i] << std::endl;
            const char* addr = peer_addresses[i];
            string addr_str(addr, strlen(addr));
            peers_[i]->Connect(addr_str);
        }
  }

  chunk(this->node_rank_, &own_offs_, &own_size_);

  for (int peer = 0; peer < peers_.size(); ++peer) {
    std::cout << "peer: " << peer << ", " << peers_.size() << std::endl;
    if (peer == this->node_rank_) {
      // Chunk for which we are master, connected to all peers. Loops must be
      // imbricated to have buffers created in the same order on all boxes.
      for (int i = 0; i < peers_.size(); ++i) {
        if (i != this->node_rank_) {
          CreateMasterBuffers(i);
        }
      }
    } else {
      // Other chunks are connected to their respective masters
      CreateWorkerBuffers(peer);
    }
  }

  return true;
}

void RDMACommunicator::sync()  {

  if (this->cluster_size_ == 1) return;

  // Send weights to each peer
  int peer = this->node_rank_ + 1;  // To avoid all sending to same peer at the same time
  for (int n = 0; n < peers_.size() - 1; ++n) {
    if (peer == peers_.size()) {
      peer = 0;
    }
    data_send_[peer]->Write();
    peer++;
  }
}

void RDMACommunicator::chunk(int peer, size_t* offs, size_t* size) {
  // TODO align chunks to page size?
  size_t start = (peer + 0) * size_ / peers_.size();
  size_t until = (peer + 1) * size_ / peers_.size();
  *offs = start;
  *size = until - start;
}

void RDMACommunicator::CreateMasterBuffers(int peer) {

  RDMAChannel* channel = peers_[peer].get();
  size_t size = own_size_; //  * sizeof(Dtype);

  std::cout << "CreateMasterBuffers, size: " << size << std::endl;

  // Send data from local (rank_) to remote (peer)
  uint8_t* data = reinterpret_cast<uint8_t*>(data_ + own_offs_);
  data_send_[peer].reset(new RDMABuffer(channel, data, size));

}

void RDMACommunicator::CreateWorkerBuffers(int peer) {

  RDMAChannel* channel = peers_[peer].get();
  size_t offs, size;
  chunk(peer, &offs, &size);
  std::cout << "CreateWorkerBuffers, size: " << size << std::endl;
  // size *= sizeof(Dtype);

  // Recv data from remote (peer) to local (rank_)
  uint8_t* data = reinterpret_cast<uint8_t*>(data_ + offs);
  data_recv_[peer].reset(new RDMABuffer(channel, data, size));

}

void RDMACommunicator::allSum(int & v){
}

void RDMACommunicator::allSum(float * v, int nv){

  float* image = (float*) (data_ + own_offs_);
  for(int i=0; i < nv; i++){
      image[i] = v[i];
  }

  sync();

  for (int n = 0; n < peers_.size() - 1; ++n) {
#ifdef DEBUG
    RDMABuffer* buffer = adapter_->received().pop();
    bool ok = false;
    for (int i = 0; i < data_recv_.size(); ++i) {
      if (buffer == data_recv_[i].get()) {
        ok = true;
      }
    }
    CHECK(ok);
#else
    RDMABuffer* buffer = adapter_->received().pop();
    float* f = (float*) buffer->addr();
    int nf = buffer->size()/sizeof(float);
    std::cout << nv << ", " << nf << ", " << v[nv-1] << ", " << f[nv-1] << std::endl;
    for(int i=0; i < nv; i++){
      v[i] += f[i];
    }
#endif
  }
}

/*
void CommunicatorRDMA::allSum(thrust::device_vector<cusp::complex<float> > & x){
}

void CommunicatorRDMA::allSum(const DeviceRange<cusp::complex<float> > & x){
}

void CommunicatorRDMA::allSum(cusp::array1d<cusp::complex<float>,
     cusp::device_memory> & x){
}

void CommunicatorRDMA::allSum(thrust::device_vector<float > & x){
}

*/
  
