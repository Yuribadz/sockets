
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <memory>

#include "tcpiomsg.hpp"

class ServerClient {
 public:
  virtual void deliver(std::unique_ptr<AbstractIoMsg> const &msg) = 0;
  virtual ~ServerClient() {
  };
};

typedef std::shared_ptr<ServerClient> client_ptr;



#endif /*CLIENT_HPP */
