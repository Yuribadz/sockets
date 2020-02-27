
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <memory>
#include "io_msg.hpp"

class ServerClient {
 public:
  virtual ~ServerClient() {
  }
  virtual void deliver(const msg &msg) = 0;
};

typedef std::shared_ptr<ServerClient> client_ptr;



#endif /*CLIENT_HPP */
