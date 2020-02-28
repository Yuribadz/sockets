#ifndef IO_MSG_HPP
#define IO_MSG_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <algorithm>
#include <iostream>

namespace {
constexpr std::size_t header_length = 4;
constexpr std::size_t max_body_length = 512;
}

template<std::size_t header_length, std::size_t max_body_length>
class io_msg {
 public:

  const char* data() const {
    return m_data.data();
  }

  char* data() {
    return m_data.data();
  }

  std::size_t length() const {
    return header_length + m_body_length;
  }

  const char* body() const {
    return m_data.data() + header_length;
  }

  char* body() {
    return m_data.data() + header_length;
  }

  std::size_t body_length() const {
    return m_body_length;
  }

  void body_length(std::size_t new_length) {
    m_body_length = new_length;
    if (m_body_length > max_body_length)
      m_body_length = max_body_length;
  }

  bool decode_header() {
    std::array<char,header_length + 1> header;
    std::copy_n(m_data.begin(), header_length + 1, header.begin());
    m_body_length = std::stoi(header.data());
    if (m_body_length > max_body_length) {
      m_body_length = 0;
      return false;
    }
    return true;
  }

  void encode_header() {
    const std::string body_count(std::to_string(m_body_length));
    std::copy_n(body_count.begin(), header_length, m_data.begin());
  }

 private:
  std::array<char, header_length + max_body_length> m_data;
  std::size_t m_body_length;
};

using msg = io_msg<header_length, max_body_length>;
using task_queue = std::deque<msg>;
#endif
