//
// Created by yurib on 25.03.2020.
//

#include "clientflow.hpp"
void ClientFlow::write(const IoMsg &msg) {
  asio::post(io_context, [this, msg]() {
    bool write_in_progress = !write_msgs.empty();
    write_msgs.push_back(msg);
    if (!write_in_progress) {
      do_write();
    }
  });
}

void ClientFlow::close() {
  asio::post(io_context, [this]() {
    socket.close();
  });
}

void ClientFlow::do_read_header() {
  auto rb_or_close = [this](std::error_code ec, std::size_t /*length*/) {
    if (!ec && tcpio::decode_header(read_msg)) {
      do_read_body();
    } else {
      socket.close();
    }
  };
  asio::async_read(socket, asio::buffer(read_msg.data.data(), 4),
                   rb_or_close);
}

void ClientFlow::do_read_body() {
  asio::async_read(socket,
                   asio::buffer(read_msg.body(), read_msg.body_len),
                   [this](std::error_code ec, std::size_t /*length*/) {
                     if (!ec) {
                       std::cout.write(read_msg.body(), read_msg.body_len);
                       std::cout << "\n";
                       do_read_header();
                     } else {
                       socket.close();
                     }
                   });
}

void ClientFlow::do_write() {
  asio::async_write(
      socket,
      asio::buffer(write_msgs.front().data.data(),
                   write_msgs.front().length()),
      [this](std::error_code ec, std::size_t /*length*/) {
        if (!ec) {
          write_msgs.pop_front();
          if (!write_msgs.empty()) {
            do_write();
          }
        } else {
          socket.close();
        }
      });
}