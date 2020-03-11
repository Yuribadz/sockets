#ifndef ABSTRACTCLIENTCLIST_HPP
#define ABSTRACTCLIENTCLIST_HPP

#include <set>
#include "serverclient.hpp"
#include "tcpiomsg.hpp"
class AbstractClientsList {
 public:
  virtual void join(client_ptr client) = 0;

  virtual void leave(client_ptr client) = 0;

  virtual void deliver(std::unique_ptr<AbstractIoMsg> const& msg) = 0;
  virtual ~AbstractClientsList() {
  }

};

#endif /* INC_ABSTRACTCLIENTCLIST_HPP_ */
