#ifndef SOCKETS_CLIENTFLOW_HPP
#define SOCKETS_CLIENTFLOW_HPP

#include "asio.hpp"
#include "tcpdecoder.hpp"
#include <iostream>
#include <deque>

using asio::ip::tcp;

class ClientFlow {

 public:
  ClientFlow() = delete;

  explicit ClientFlow(asio::io_context &context,
                      tcp::socket &socket_ref)
      : io_context(context),
        socket(socket_ref),
        read_msg(
            IoMsg(as_integer(TcpMsgHeaderLength::HEADER_LENGTH),
                  as_integer(TcpMsgMaxBodyLength::MAX_BODY_LENGTH))) {};

  void write(const IoMsg &msg);
  void close();
  void do_read_header();
  void do_read_body();
  void do_write();

  asio::io_context &io_context;
  tcp::socket &socket;
  IoMsg read_msg;
  std::deque<IoMsg> write_msgs;
};

#endif //SOCKETS_CLIENTFLOW_HPP
