#ifndef TCPDECODER_HPP
#define TCPDECODER_HPP

#include "abstractdecoder.hpp"
#include "tcpiomsg.hpp"
#include <array>
#include <memory>

namespace {
size_t header_length = as_integer(TcpIoMsg::TcpMsgHeaderLength::HEADER_LENGTH);
size_t max_body_length = as_integer(
    TcpIoMsg::TcpMsgMaxBodyLength::MAX_BODY_LENGTH);
}
class TcpDecoder : public AbstractDecoder {

  bool decode_header(std::unique_ptr<AbstractIoMsg> const &msg) override {
    std::array<char, as_integer(TcpIoMsg::TcpMsgHeaderLength::HEADER_LENGTH) + 1> header;
    std::copy_n(msg->data(), header_length + 1, header.begin());
    msg->body_length(std::stoi(header.data()));
    if (msg->body_length() > max_body_length) {
      msg->body_length(0);
      return false;
    }
    return true;
  }
};

#endif /* INC_TCPDECODER_HPP_ */
