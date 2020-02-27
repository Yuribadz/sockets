
#ifndef ABSTRACTCLIENTCLIST_HPP
#define ABSTRACTCLIENTCLIST_HPP

#include <set>
#include "serverclient.hpp"

class AbstractClientsList {
 public:
  virtual void join(client_ptr client);

  virtual void leave(client_ptr client);

  virtual void deliver(const msg &msg);

  virtual ~AbstractClientsList();
};

#endif /* INC_ABSTRACTCLIENTCLIST_HPP_ */
