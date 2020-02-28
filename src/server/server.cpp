
#include <iostream>
#include <list>
#include <cstdio>
#include "asio.hpp"
#include "tcpserver.hpp"
using asio::ip::tcp;


int main(int argc, char *argv[]) {
  try {
    if (argc < 2) {
      std::cerr << "Usage: chat_server <port> [<port> ...]\n";
      return 1;
    }

    asio::io_context io_context;

    std::list<TcpServer> servers;
    for (int i = 1; i < argc; ++i) {
      tcp::endpoint endpoint(tcp::v4(), std::stoi(argv[i]));
      servers.emplace_back(io_context, endpoint);
    }

    io_context.run();
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  return 0;
}
