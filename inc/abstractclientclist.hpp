#ifndef ABSTRACTCLIENTCLIST_HPP
#define ABSTRACTCLIENTCLIST_HPP

#include <set>
#include "serverclient.hpp"
#include "io_msg.hpp"
class AbstractClientsList {
 public:
  virtual void join(client_ptr client) = 0;

  virtual void leave(client_ptr client) = 0;

  virtual void deliver(const msg &msg) = 0;
  virtual ~AbstractClientsList() {
  }

};

#endif /* INC_ABSTRACTCLIENTCLIST_HPP_ */
