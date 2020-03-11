
#ifndef TCPENCODER_HPP
#define TCPENCODER_HPP

#include "abstractencoder.hpp"
#include "tcpiomsg.hpp"

class TcpEncoder: public AbstractEncoder
{
  virtual void encode_header(TcpIoMsg &msg){
      const std::string body_count(std::to_string(msg.body_length()));
      std::copy_n(body_count.begin(), static_cast<int>header_length, msg.msgData.begin());
  }
};


#endif
