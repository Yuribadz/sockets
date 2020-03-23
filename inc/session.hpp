#ifndef INC_SESSION_HPP
#define INC_SESSION_HPP

#include "serverclient.hpp"
#include "asio.hpp"
#include "tcpclientlist.hpp"
#include "tcpencoder.hpp"
#include "tcpdecoder.hpp"
#include <memory>

using asio::ip::tcp;

class Session : public ServerClient,
    public std::enable_shared_from_this<Session> {
 public:
  Session(tcp::socket socket, std::unique_ptr<AbstractClientsList> clients)
      :
      socket_(std::move(socket)),
      clients_(std::move(clients)),
      encoder_(std::make_unique<TcpEncoder>()),
      decoder_(std::make_unique<TcpDecoder>()),
      read_msg_(IoMsg(4,512)) {
  }

  void start() {
    clients_->join(shared_from_this());
    do_read_header();
  }

  void deliver(const IoMsg &msg) override {
    std::cout << "deliver called" << "\n";
    bool write_in_progress = !write_msgs_.empty();
       write_msgs_.push_back(msg);
    if (!write_in_progress) {
      do_write();
    }
  }

 private:
  void do_read_header() {
    std::cout << "Do read_header callback" << "\n";
    auto self(shared_from_this());
    auto read_cb = [this, self](std::error_code ec, std::size_t /*length*/) {
      if (!ec && decoder_->decode_header(read_msg_)) {
        do_read_body();
      } else {
        clients_->leave(shared_from_this());
      }
    };
    asio::async_read(socket_, asio::buffer(read_msg_.data.data(), 4), read_cb);
  }

  void do_read_body() {
    std::cout << "Do read_body callback" << "\n";
    auto self(shared_from_this());
    auto read_cb = [this, self](std::error_code ec, std::size_t /*length*/) {
      if (!ec) {
        clients_->deliver(read_msg_);
        do_read_header();
      } else {
        clients_->leave(shared_from_this());
      }
    };
    asio::async_read(socket_,
                     asio::buffer(read_msg_.body(), read_msg_.body_len),
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
        clients_->leave(shared_from_this());
      }
    };
    asio::async_write(
        socket_,
        asio::buffer(write_msgs_.front().data.data(),
                     write_msgs_.front().length()),
        write_cb);
  }

  tcp::socket socket_;
  std::unique_ptr<AbstractClientsList> clients_;
  std::unique_ptr<AbstractEncoder> encoder_;
  std::unique_ptr<AbstractDecoder> decoder_;
  IoMsg read_msg_;
  std::deque<IoMsg> write_msgs_;
};

#endif
