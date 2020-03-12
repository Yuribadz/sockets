#include "tcpclientlist.hpp"
#include <cstring>

namespace {
size_t tcpMsgLength = as_integer(TcpIoMsg::TcpMsgHeaderLength::HEADER_LENGTH)
    + as_integer(TcpIoMsg::TcpMsgMaxBodyLength::MAX_BODY_LENGTH);
}
void TcpClientsList::join(client_ptr client) {
  m_Clients.insert(client);
  for (auto const &msg : m_recent_msgs)
    client->deliver(msg);
}

void TcpClientsList::leave(client_ptr client) {
  m_Clients.erase(client);
}

void TcpClientsList::deliver(std::unique_ptr<AbstractIoMsg> const &msg) {

  std::unique_ptr<AbstractIoMsg> sent = std::make_unique<TcpIoMsg>();
  sent->body_length(msg->body_length());
  std::strncpy(sent->data(), msg->const_data(), tcpMsgLength);
  m_recent_msgs.push_back(sent);
  while (m_recent_msgs.size() > m_max_m_recent_msgs_)
    m_recent_msgs.pop_front();

  for (auto const &client : m_Clients)
    client->deliver(msg);
}
