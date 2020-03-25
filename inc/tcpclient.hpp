
#ifndef SOCKETS_TCPCLIENT_HPP
#define SOCKETS_TCPCLIENT_HPP
#include "iomsg.hpp"
#include "asio.hpp"
#include "tcpdecoder.hpp"
#include "tcpencoder.hpp"
#include "clientflow.hpp"

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <functional>

using asio::ip::tcp;

class TcpClient {
 public:
  TcpClient(asio::io_context &io_context,
            const tcp::resolver::results_type &endpoints)
      :
      socket_(io_context),
      flow_(ClientFlow(io_context, socket_)) {
    asio::async_connect(socket_, endpoints,
                        [this](std::error_code ec, tcp::endpoint) {
                          if (!ec) {
                            flow_.do_read_header();
                          }
                        });
  }
  void write(const IoMsg &msg) {
    flow_.write(msg);
  };
  void close() {
    flow_.close();
  };

 private:
  tcp::socket socket_;
  ClientFlow flow_;
};

#endif //SOCKETS_TCPCLIENT_HPP
