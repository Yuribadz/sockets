#ifndef TCPDECODER_HPP
#define TCPDECODER_HPP

#include <array>
#include "iomsg.hpp"

namespace tcpio {
bool decode_header(IoMsg &msg) {
  std::array<char, as_integer(TcpMsgHeaderLength::HEADER_LENGTH) + 1> header;
  std::copy_n(msg.data.data(), msg.HEADER_LENGTH + 1, header.begin());
  msg.body_len = std::stoi(header.data());
  if (msg.body_len > msg.MAX_BODY_LENGTH) {
    msg.body_len = 0;
    return false;
  }
  return true;
}

}
#endif /* INC_TCPDECODER_HPP_ */
