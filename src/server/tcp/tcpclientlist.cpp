#include "tcpclientlist.hpp"
#include <cstring>

void TcpClientsList::join(client_ptr client) {
  m_Clients.insert(client);
  for (auto const &msg : m_recent_msgs)
    client->deliver(msg);
}

void TcpClientsList::leave(client_ptr client) {
  m_Clients.erase(client);
}

void TcpClientsList::deliver(const IoMsg &msg) {

  m_recent_msgs.push_back(msg);
  while (m_recent_msgs.size() > m_max_m_recent_msgs_)
    m_recent_msgs.pop_front();

  for (auto &client : m_Clients)
    client->deliver(msg);
}
