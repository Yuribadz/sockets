#include "tcpencoder.hpp"

namespace tcpio {
void encode_header(IoMsg &msg) {
  const std::string body_count(std::to_string(msg.body_len));
  std::copy_n(body_count.begin(), msg.HEADER_LENGTH, msg.data.data());
}
}
