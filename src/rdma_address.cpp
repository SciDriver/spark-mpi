#include "rdma_address.hpp"

#include "util/socket.cpp"

using namespace std;
using namespace caffe;

// Handle each peer connection in their own handlers
void *address_client_handler(void *metadata) {

  struct connection_details * cd = (struct connection_details*) metadata;
  struct message_header mh;

  while (1) {

    receive_message_header(cd->serving_fd, &mh);
    // FIXME: The condition where socket server gets a connection
    // from the peer but the user didn't allocate a SocketChannel
    // for the peer.This can even happen if adapter is instantiated
    // first and SocketChannel is allocated for the peer subsequently
    // For now assume the user will maintain the right order i.e
    // allocate a SocketChannel for all peers and then instantiate
    // the adapter.
    /*      if(cd->sa->channels.size() < mh.rank || cd->sq->channels.at(mh.rank) == NULL){
            printf("ERROR:No SocketChannel assigned for the peer with rank [%d]...terminating the thread handler\n", mh.rank);
            // FIXME: Notify the client and exit
            pthread_exit();
            }
    */
 
    // From the peer rank locate the local SocketChannel for that
    // peer and sets it's serving_fd
    RDMAAddressServer* addr_srv = (RDMAAddressServer*) cd->sa;
    SocketChannel* sc = addr_srv->channels->at(mh.rank).get();
    sc->serving_fd = cd->serving_fd;
    sc->client_fd = cd->serving_fd;

    uint8_t* read_buffer = new uint8_t[mh.size];
    uint8_t* marker = read_buffer;
    int cur_cnt = 0;
    int max_buff = 0;
    while (cur_cnt < mh.size) {
      if ((mh.size - cur_cnt) > 256)
        max_buff = 256;
      else
        max_buff = mh.size - cur_cnt;

      int n = read(sc->serving_fd, marker, max_buff);
      marker = marker + n;
      cur_cnt = cur_cnt + n;
    }

    // Wrap the received message in an object QueuedMessage and
    // push it to the local receive queue of the peer's
    // SocketChannel
    QueuedMessage* mq = new QueuedMessage(mh.type,
                                          mh.size, 
					  read_buffer);

    std::cout << "address_client  handler, rank: " << mh.rank 
	      << ", size: " << mq->size 
	      << ", buffer: " << (char*) mq->buffer 
	      << std::endl;

    sc->receive_queue.push(mq);

    addr_srv->counter++; 

    if(addr_srv->counter == addr_srv->channels->size()){
      addr_srv->counter = 0;
      addr_srv->send_addrs();
    }

    break;
  }

  return NULL;
}

void *address_srvr(void *metadata) {

  RDMAAddressServer* sa = reinterpret_cast<RDMAAddressServer*>(metadata);
  // Open a server socket
  int serv_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (serv_fd < 0) {
    LOG(ERROR) << "ERROR: Could not open server socket " << INADDR_ANY;
  }

  // Initialize the socket structs
  struct sockaddr_in serv_addr;
  bzero(reinterpret_cast<char *>(&serv_addr), sizeof(serv_addr));
  // Using internet domain sockets
  serv_addr.sin_family = AF_INET;
  // Get server ip
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  // Allow the server to be assigned a random free port
  serv_addr.sin_port = 0;
  char hostname[256];
  // Bind the server socket to the server host
  if (bind(serv_fd, (struct sockaddr *) &serv_addr,
           sizeof(serv_addr)) < 0) {
    gethostnamebysinaddr(serv_addr, hostname, 256);
    LOG(ERROR) << "ERROR: Unable to bind socket fd to server ["
               << hostname <<"]";
  }
  // Check what port number was assigned to the socket server
  struct sockaddr_in sin;
  socklen_t len = sizeof(sin);
  int port_no = 0;
  if (getsockname(serv_fd, (struct sockaddr *)&sin, &len) == -1) {
    LOG(ERROR) << "ERROR: getting server socket details";
  } else {
    port_no = ntohs(sin.sin_port);
    LOG(INFO) << "Assigned socket server port [" << port_no << "]";
  }
  char self_name[256];
  if (gethostname(self_name, 256) == -1)
    LOG(ERROR) << "ERROR: Could not determine self hostname ["
               << errno <<"]";

  // Set the assigned socket server port no in the socket adapter
  sa->port = port_no;
  // Listen to incoming connections on this server socket
  // (5 simultaneously)
  listen(serv_fd, 5);
  gethostnamebysinaddr(serv_addr, hostname, 256);
  LOG(INFO) << "Socket Server ready [" << hostname << "]";
  // Accept the incoming connection on the socket server
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);
  int serving_fd;
  pthread_t thread_id;
  while ((serving_fd = accept(serv_fd, (struct sockaddr *)
                              &client_addr, &client_addr_len))) {
    char client_hostname[256];
    gethostnamebysinaddr(client_addr, client_hostname, 256);
    struct connection_details* cd = new struct connection_details;
    cd->serving_fd = serving_fd;
    cd->sa = sa;
    LOG(INFO) << "Accepted the connection from client ["
              << client_hostname << "]";
    // Create a thread per peer connection to handle
    // their communication
    if (pthread_create(&thread_id, NULL,
                       address_client_handler,
                       reinterpret_cast<void*>(cd)) < 0) {
      LOG(ERROR) << "ERROR: Could not create thread for the "
                 << "incoming client connection ["
                 << client_hostname << "]";
      continue;
    }
  }
  if (serving_fd < 0) {
    LOG(ERROR) << "ERROR: Could not accept incoming "
               << "connection to socket server";
  }
  // FIXME: Write threads and socket server exit/cleanup logic
  return NULL;
}


RDMAAddressServer::RDMAAddressServer(int size) {

  counter = 0;

  channels = new vector<boost::shared_ptr<SocketChannel> >(size);

  for (int i = 0; i < size; i++) {
    (*channels)[i].reset(new SocketChannel());
  }

  // SocketAdapter

  // Let SocketAdapter know the allocated SocketChannels
  // for the peers so that when the peers connect to the local
  // socket server, we can locate their SocketChannels based on
  // their ranks, received in the message header. Each vector
  // index here corresponds to the peer rank
  this->port = 0;
  // this->channels = channels;
  // Start socket server
  start_sockt_srvr();
  // Wait till the socket server has
  // started with a valid port
  while (true) {
    LOG(INFO) << "Waiting for valid port ["<< this->port <<"]";
    if (this->port != 0) {
      break;
    }
    else {
      usleep(10000);
    }
  }
  LOG(INFO) << "Valid port found ["<< this->port << "]";
}

RDMAAddressServer::~RDMAAddressServer(){

  for (int i = 0; i < channels->size(); i++) {
    (*channels)[i].reset();
  }
  delete channels;
}

void RDMAAddressServer::start_sockt_srvr() {
  pthread_t thread_id;
  // Start the socket server in it's own thread as accept
  // is a blocking call
  if (pthread_create(&thread_id, NULL, address_srvr,
                     reinterpret_cast<void*>(this)) < 0) {
    LOG(ERROR) << "ERROR: Could not start the socket server";
  }
}

void RDMAAddressServer::send_addrs() {

  int size = channels->size();

  vector< vector<string> > addrs(size);
  for(int i = 0; i < size; i++){
    addrs[i].resize(size);
  }

  for(int i = 0; i < size; i++){

    SocketChannel* sc = channels->at(i).get();
    QueuedMessage* qm = sc->receive_queue.pop();

    int index = 0;
    for(int j = 0; j < size; j++){
      if(i == j){
	addrs[j][i] = "";
	continue;
      }
      char dest[24];
      char* src =  (char*) qm->buffer + 24*index;
      strncpy(dest, src, 24);
      addrs[j][i].assign(dest, 24);
      index++;
    }

    delete [] qm->buffer;
    delete qm;

  }

  for(int i = 0; i < size; i++){

    string buffer;
    for(int j = 0; j < size; j++){
      buffer += addrs[i][j];
    }
    // std::cout << i << ", buffer: " << buffer << ", size: " << buffer.size() << std::endl;
    SocketChannel* sc = channels->at(i).get();
    caffe::SocketBuffer sb(i, sc,  (uint8_t*) buffer.c_str(), buffer.size(), (uint8_t*) buffer.c_str());
    sb.Write();    
  }

}

//

RDMAAddressClient::RDMAAddressClient(int r) 
  : rank(r) {
}

RDMAAddressClient::~RDMAAddressClient(){
}

void RDMAAddressClient::send_addrs(string& rdma_addrs){

  uint8_t* buffer = (uint8_t*) rdma_addrs.c_str();
  caffe::SocketBuffer sb(rank, this,  buffer, rdma_addrs.size(), buffer);
  sb.Write();

}

void RDMAAddressClient::recv_addrs(string& rdma_addrs){

    struct message_header mh;

    receive_message_header(client_fd, &mh);
    // FIXME: The condition where socket server gets a connection
    // from the peer but the user didn't allocate a SocketChannel
    // for the peer.This can even happen if adapter is instantiated
    // first and SocketChannel is allocated for the peer subsequently
    // For now assume the user will maintain the right order i.e
    // allocate a SocketChannel for all peers and then instantiate
    // the adapter.
 
    // From the peer rank locate the local SocketChannel for that
    // peer and sets it's serving_fd

    uint8_t* read_buffer = new uint8_t[mh.size];
    uint8_t* marker = read_buffer;
    int cur_cnt = 0;
    int max_buff = 0;
    while (cur_cnt < mh.size) {
      if ((mh.size - cur_cnt) > 256)
        max_buff = 256;
      else
        max_buff = mh.size - cur_cnt;

      int n = read(client_fd, marker, max_buff);
      marker = marker + n;
      cur_cnt = cur_cnt + n;
    }

    // Wrap the received message in an object QueuedMessage and
    // push it to the local receive queue of the peer's
    // SocketChannel

    rdma_addrs.assign((char*) read_buffer, mh.size);

    /*
    std::cout << "read, rank: " << mh.rank 
	      << ", size: " << mh.size 
              << ", buffer: " << rdma_addrs
	      << std::endl;
    */

    return;
}




