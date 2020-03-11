#ifndef INC_SESSION_HPP
#define INC_SESSION_HPP

#include "serverclient.hpp"
#include "asio.hpp"
#include "tcpclientlist.hpp"
#include <memory>

using asio::ip::tcp;

class Session : public ServerClient,
    public std::enable_shared_from_this<Session> {
 public:
  Session(tcp::socket socket, AbstractClientsList &clients)
      :
      socket_(std::move(socket)),
      clients_(clients),
      read_msg_(std::make_unique<tcpmsg>()) {
  }

  void start() {
    clients_.join(shared_from_this());
    do_read_header();
  }

  void deliver(AbstractIoMsg const &msg) override {
    std::cout << "deliver called" << "\n";
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg.get());
    if (!write_in_progress) {
      do_write();
    }
  }

 private:
  void do_read_header() {
    std::cout << "Do read_header callback" << "\n";
    auto self(shared_from_this());
    auto read_cb = [this, self](std::error_code ec, std::size_t /*length*/) {
      if (!ec && read_msg_->decode_header()) {
        do_read_body();
      } else {
        clients_.leave(shared_from_this());
      }
    };
    asio::async_read(socket_, asio::buffer(read_msg_->data(), 4), read_cb);
  }

  void do_read_body() {
    std::cout << "Do read_body callback" << "\n";
    auto self(shared_from_this());
    auto read_cb = [this, self](std::error_code ec, std::size_t /*length*/) {
      if (!ec) {
        clients_.deliver(read_msg_);
        do_read_header();
      } else {
        clients_.leave(shared_from_this());
      }
    };
    asio::async_read(socket_,
                     asio::buffer(read_msg_->body(), read_msg_->body_length()),
                     read_cb);
  }

  void do_write() {
    std::cout << "Do write callback" << "\n";
    auto self(shared_from_this());
    auto write_cb = [this, self](std::error_code ec, std::size_t /*length*/) {
      if (!ec) {
        write_msgs_.pop_front();
        if (!write_msgs_.empty()) {
          do_write();
        }
      } else {
        clients_.leave(shared_from_this());
      }
    };
    asio::async_write(
        socket_,
        asio::buffer(write_msgs_.front()->data(),
                     write_msgs_.front()->length()),
        write_cb);
  }

  tcp::socket socket_;
  AbstractClientsList &clients_;
  std::unique_ptr<AbstractIoMsg> read_msg_;
  task_queue write_msgs_;
};

#endif
