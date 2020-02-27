#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

#include <memory>

#include "asio.hpp"
#include "session.hpp"


using namespace asio::ip;
class TcpServer {
 public:
  TcpServer(asio::io_context &io_context, const tcp::endpoint &endpoint)
      :
      acceptor_(io_context, endpoint) {
    do_accept();
  }

 private:
  void do_accept() {
    auto accept_cb = [this](std::error_code ec, tcp::socket socket) {
      if (!ec) {
        std::make_shared<Session>(std::move(socket), clients_)->start();
      }

      do_accept();
    };
    acceptor_.async_accept(accept_cb);
  }

  tcp::acceptor acceptor_;
  ClientsList clients_;
};
#endif
