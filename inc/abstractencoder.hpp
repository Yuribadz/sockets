#ifndef ABSTRACTENCODER_HPP
#define ABSTRACTENCODER_HPP

#include "abstractiomsg.hpp"
class AbstractEncoder {
 public:
  virtual void encode_header(IoMsg &msg) = 0;
  virtual ~AbstractEncoder() = default;
};

#endif /* ABSTRACTENCODER_HPP */
