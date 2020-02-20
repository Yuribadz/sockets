#include <iostream>
#include "asio.hpp"
#include <functional>

class Printer {
 public:
  Printer(asio::io_context &io)
      :
      _strand(io),
      _timer1(io, asio::chrono::seconds(1)),
      _timer2(io, asio::chrono::seconds(1)),
      _count(0) {
    _timer1.async_wait(
        asio::bind_executor(_strand, std::bind(&Printer::print1, this)));
    _timer2.async_wait(
        asio::bind_executor(_strand, std::bind(&Printer::print2, this)));
  }
  ~Printer() {
    std::cout << "Final count is " << _count << "\n";
  }

  void print1() {
    if (_count < 10) {
      std::cout << "Timer 1: " << _count << "\n";
      ++_count;
      _timer1.expires_at(_timer1.expires_at() + asio::chrono::seconds(1));
      _timer1.async_wait(
          asio::bind_executor(_strand, std::bind(&Printer::print1, this)));
    }
  }
  void print2() {
    if (_count < 10) {
      std::cout << "Timer 1: " << _count << "\n";
      ++_count;
      _timer2.expires_at(_timer2.expires_at() + asio::chrono::seconds(1));
      _timer2.async_wait(
          asio::bind_executor(_strand, std::bind(&Printer::print2, this)));
    }
  }
 private:
  asio::io_context::strand _strand;
  asio::steady_timer _timer1;
  asio::steady_timer _timer2;
  int _count;
};

int main() {
  asio::io_context io;
  Printer p(io);
  asio::thread([&]{io.run();});
  io.run();

  return 0;
}
