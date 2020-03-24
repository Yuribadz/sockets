#ifndef ABSTRACTCLIENTCLIST_HPP
#define ABSTRACTCLIENTCLIST_HPP

#include <set>

#include "iomsg.hpp"
#include "serverclient.hpp"
class AbstractClientsList {
 public:
  virtual void join(client_ptr client) = 0;

  virtual void leave(client_ptr client) = 0;

  virtual void deliver(const IoMsg &msg) = 0;
  virtual ~AbstractClientsList() {
  }

};

#endif /* INC_ABSTRACTCLIENTCLIST_HPP_ */
