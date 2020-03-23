
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <memory>

#include "abstractiomsg.hpp"

class ServerClient {
 public:
  virtual void deliver(IoMsg const &msg) = 0;
  virtual ~ServerClient() {
  };
};

typedef std::shared_ptr<ServerClient> client_ptr;



#endif /*CLIENT_HPP */
