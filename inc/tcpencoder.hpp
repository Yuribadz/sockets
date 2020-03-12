
#ifndef TCPENCODER_HPP
#define TCPENCODER_HPP

#include "abstractencoder.hpp"
#include "tcpiomsg.hpp"

class TcpEncoder: public AbstractEncoder
{
 public:
  virtual void encode_header(std::unique_ptr<AbstractIoMsg> const &msg){
    int header_length =
            as_integer(TcpIoMsg::TcpMsgHeaderLength::HEADER_LENGTH);
      const std::string body_count(std::to_string(msg->body_length()));
      std::copy_n(body_count.begin(), header_length, msg->data());
  }
};


#endif
