#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <functional>

#include "iomsg.hpp"
#include "asio.hpp"
#include "tcpdecoder.hpp"
#include "tcpencoder.hpp"

using asio::ip::tcp;

using EncoderFunction = std::function<void (IoMsg &msg)>;
using DecoderFunction = std::function<bool (IoMsg &msg)>;

class chat_client {
 public:
  chat_client(asio::io_context &io_context,
              const tcp::resolver::results_type &endpoints)
      :
      encoder_(std::bind(&tcpio::encode_header, std::placeholders::_1)),
      decoder_(std::bind(&tcpio::decode_header, std::placeholders::_1)),
      io_context_(io_context),
      socket_(io_context),
      read_msg_(
          IoMsg(as_integer(TcpMsgHeaderLength::HEADER_LENGTH),
                as_integer(TcpMsgMaxBodyLength::MAX_BODY_LENGTH))) {
    do_connect(endpoints);
  }

  EncoderFunction encoder_;
  DecoderFunction decoder_;
  void write(const IoMsg &msg) {
    asio::post(io_context_, [this, msg]() {
      bool write_in_progress = !write_msgs_.empty();
      write_msgs_.push_back(msg);
      if (!write_in_progress) {
        do_write();
      }
    });
  }

  void close() {
    asio::post(io_context_, [this]() {
      socket_.close();
    });
  }

 private:
  void do_connect(const tcp::resolver::results_type &endpoints) {
    asio::async_connect(socket_, endpoints,
                        [this](std::error_code ec, tcp::endpoint) {
                          if (!ec) {
                            do_read_header();
                          }
                        });
  }

  void do_read_header() {
    auto rb_or_close = [this](std::error_code ec, std::size_t /*length*/) {
      if (!ec && decoder_(read_msg_)) {
        do_read_body();
      } else {
        socket_.close();
      }
    };
    asio::async_read(socket_, asio::buffer(read_msg_.data.data(), 4),
                     rb_or_close);
  }

  void do_read_body() {
    asio::async_read(socket_,
                     asio::buffer(read_msg_.body(), read_msg_.body_len),
                     [this](std::error_code ec, std::size_t /*length*/) {
                       if (!ec) {
                         std::cout.write(read_msg_.body(), read_msg_.body_len);
                         std::cout << "\n";
                         do_read_header();
                       } else {
                         socket_.close();
                       }
                     });
  }

  void do_write() {
    asio::async_write(
        socket_,
        asio::buffer(write_msgs_.front().data.data(),
                     write_msgs_.front().length()),
        [this](std::error_code ec, std::size_t /*length*/) {
          if (!ec) {
            write_msgs_.pop_front();
            if (!write_msgs_.empty()) {
              do_write();
            }
          } else {
            socket_.close();
          }
        });
  }

 private:
  asio::io_context &io_context_;
  tcp::socket socket_;
  IoMsg read_msg_;
  std::deque<IoMsg> write_msgs_;
};

int main(int argc, char *argv[]) {
  try {
    if (argc != 3) {
      std::cerr << "Usage: chat_client <host> <port>\n";
      return 1;
    }

    asio::io_context io_context;

    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], argv[2]);
    chat_client c(io_context, endpoints);

    std::thread t([&io_context]() {
      io_context.run();
    });

    char line[512 + 1];
    while (std::cin.getline(line, 512 + 1)) {
      IoMsg message(4, 512);
      message.body_len = std::strlen(line);
      std::memcpy(message.body(), line, message.body_len);
      c.encoder_(message);
      c.write(message);
    }

    c.close();
    t.join();
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
