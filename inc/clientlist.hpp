#ifndef CLIENTLIST_HPP
#define CLIENTLIST_HPP

#include <set>

#include "serverclient.hpp"

class ClientsList {
 public:
  void join(client_ptr client) {
    clients_.insert(client);
    for (auto msg : recent_msgs_)
      client->deliver(msg);
  }

  void leave(client_ptr client) {
    clients_.erase(client);
  }

  void deliver(const msg &msg) {
    recent_msgs_.push_back(msg);
    while (recent_msgs_.size() > max_recent_msgs)
      recent_msgs_.pop_front();

    for (auto client : clients_)
      client->deliver(msg);
  }

 protected:
  std::set<client_ptr> clients_;
  constexpr unsigned int max_recent_msgs = m;
  task_queue recent_msgs_;
};



#endif
