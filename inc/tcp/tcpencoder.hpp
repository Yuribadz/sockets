#ifndef TCPENCODER_HPP
#define TCPENCODER_HPP

#include "iomsg.hpp"
#include <string>

namespace tcpio {
void encode_header(IoMsg &msg);
}
#endif
