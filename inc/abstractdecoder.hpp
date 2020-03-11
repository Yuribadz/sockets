#ifndef ABSTRACTDECODER_HPP
#define ABSTRACTDECODER_HPP


class AbstractDecoder
{
  virtual void decode_header() = 0;
  virtual ~AbstractDecoder() = default;
};

#endif /* ABSTRACTDECODER_HPP */
