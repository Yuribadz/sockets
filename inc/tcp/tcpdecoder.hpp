#ifndef TCPDECODER_HPP
#define TCPDECODER_HPP

#include <array>
#include "iomsg.hpp"

namespace tcpio {
bool decode_header(IoMsg &msg);

}
#endif /* INC_TCPDECODER_HPP_ */
