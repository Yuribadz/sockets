#ifndef TCPCLIENTLIST_HPP
#define TCPCLIENTLIST_HPP

#include <set>

#include "serverclient.hpp"
#include "abstractclientclist.hpp"

class TcpClientsList : public AbstractClientsList {
 public:

  TcpClientsList() = delete;
  TcpClientsList(unsigned int max_recent_messages)
      :
      AbstractClientsList(),
      m_max_m_recent_msgs_(max_recent_messages) {

  }
  void join(client_ptr client) override;
  void leave(client_ptr client) override;
  void deliver(std::unique_ptr<AbstractIoMsg>const & msg) override;

 ~TcpClientsList(){};
 protected:
  std::set<client_ptr> m_Clients;
  unsigned int m_max_m_recent_msgs_;
  task_queue m_recent_msgs;
};

#endif
