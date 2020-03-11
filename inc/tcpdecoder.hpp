#ifndef TCPDECODER_HPP
#define TCPDECODER_HPP

#include "abstractdecoder.hpp"
#include "tcpiomsg.hpp"
#include <array>
class TcpDecoder : public AbstractDecoder {

  bool decode_header(TcpIoMsg &msg) override {
    int header_length =
        as_integer(TcpIoMsg::TcpMsgHeaderLength::HEADER_LENGTH);
    int max_body_length =
        as_integer(TcpIoMsg::TcpMsgMaxBodyLength::MAX_BODY_LENGTH);
    std::array<char,
        static_cast<int>(TcpIoMsg::TcpMsgHeaderLength::HEADER_LENGTH) + 1> header;
    std::copy_n(msg.msgData.begin(), header_length + 1, header.begin());
    msg.m_body_length = std::stoi(header.data());
    if (msg.m_body_length > max_body_length) {
      msg.m_body_length = 0;
      return false;
    }
    return true;
  }
};

#endif /* INC_TCPDECODER_HPP_ */
