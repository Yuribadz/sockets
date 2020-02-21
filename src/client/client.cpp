#include <iostream>
#include "asio.hpp"
#include <functional>

int main(int argc, char *argv[]) {

  try {
    if (argc != 2) {
      std::cerr << "Usage : client <host>" << "\n";
      return 1;
    }

    asio::io_context io_context;
    asio::ip::tcp::resolver resolver(io_context);
    asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(
        argv[1], "daytime");
    asio::ip::tcp::socket socket(io_context);
    asio::connect(socket, endpoints);
    for (;;)
       {
         std::array<char, 128> buf;
         asio::error_code error;

         size_t len = socket.read_some(asio::buffer(buf), error);

         if (error == asio::error::eof)
           break; // Connection closed cleanly by peer.
         else if (error)
           throw asio::system_error(error); // Some other error.

         std::cout.write(buf.data(), len);
       }
  } catch (std::exception &e) {
    std::cerr << e.what() << "\n";
  }
}
