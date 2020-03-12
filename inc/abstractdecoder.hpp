#ifndef ABSTRACTDECODER_HPP
#define ABSTRACTDECODER_HPP

#include "abstractiomsg.hpp"
#include <memory>
class AbstractDecoder {
 public:
  virtual bool decode_header(std::unique_ptr<AbstractIoMsg> const &msg) = 0;
  virtual ~AbstractDecoder() = default;
};

#endif /* ABSTRACTDECODER_HPP */
