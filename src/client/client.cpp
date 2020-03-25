#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <functional>

#include "iomsg.hpp"
#include "asio.hpp"
#include "tcpencoder.hpp"
#include "tcpclient.hpp"

using asio::ip::tcp;

int main(int argc, char *argv[]) {
  try {
    if (argc != 3) {
      std::cerr << "Usage: chat_client <host> <port>\n";
      return 1;
    }

    asio::io_context io_context;

    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], argv[2]);
    TcpClient c(io_context, endpoints);

    std::thread t([&io_context]() {
      io_context.run();
    });

    char line[512 + 1];
    while (std::cin.getline(line, 512 + 1)) {
      IoMsg message(4, 512);
      message.body_len = std::strlen(line);
      std::memcpy(message.body(), line, message.body_len);
      tcpio::encode_header(message);
      c.write(message);
    }

    c.close();
    t.join();
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
