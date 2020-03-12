#ifndef IO_MSG_HPP
#define IO_MSG_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <algorithm>
#include <iostream>

#include "abstractiomsg.hpp"

class TcpIoMsg : public AbstractIoMsg {
 public:
  enum class TcpMsgHeaderLength : int {
    HEADER_LENGTH = 4
  };
  enum class TcpMsgMaxBodyLength : int {
    MAX_BODY_LENGTH = 512
  };
  const char* const_data() const override {
    return msgData.data();
  }

  char* data() override {
    return msgData.data();
  }

  std::size_t length() const override {
    return as_integer(TcpMsgHeaderLength::HEADER_LENGTH) + m_body_length;
  }

  const char* body() const override {
    return msgData.data() + as_integer(TcpMsgHeaderLength::HEADER_LENGTH);
  }

  char* body() override {
    return msgData.data() + as_integer(TcpMsgHeaderLength::HEADER_LENGTH);
  }

  std::size_t body_length() const override {
    return m_body_length;
  }

  void body_length(std::size_t new_length) override {
    m_body_length = new_length;
    if (m_body_length > as_integer(TcpMsgMaxBodyLength::MAX_BODY_LENGTH))
      m_body_length = as_integer(TcpMsgMaxBodyLength::MAX_BODY_LENGTH);
  }
  std::array<char,
  as_integer(TcpMsgMaxBodyLength::MAX_BODY_LENGTH)
          + as_integer(TcpMsgHeaderLength::HEADER_LENGTH)> msgData;
  std::size_t m_body_length;
};

using tcp_msg_queue = std::deque<std::unique_ptr<AbstractIoMsg>>;
#endif
