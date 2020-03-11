
#ifndef ABSTRACTENCODER_HPP
#define ABSTRACTENCODER_HPP


class AbstractEncoder
{
  virtual void encode_header() = 0;
  virtual ~AbstractEncoder() = default;
};


#endif /* ABSTRACTENCODER_HPP */
