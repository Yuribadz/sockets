#ifndef ABSTRACTENCODER_HPP
#define ABSTRACTENCODER_HPP

class AbstractEncoder {
 public:
  virtual void encode_header(std::unique_ptr<AbstractIoMsg> const &msg) = 0;
  virtual ~AbstractEncoder() = default;
};

#endif /* ABSTRACTENCODER_HPP */
