#ifndef ABSTRACTIOMSG_HPP
#define ABSTRACTIOMSG_HPP

#include <vector>
#include <algorithm>

template<typename Enumeration>
constexpr auto as_integer(
    Enumeration const value) -> typename std::underlying_type<Enumeration>::type {
  static_assert(std::is_enum<Enumeration>::value, "parameter is not of type enum or enum class");
  return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}

enum class TcpMsgHeaderLength : int {
  HEADER_LENGTH = 4
};
enum class TcpMsgMaxBodyLength : int {
  MAX_BODY_LENGTH = 512
};

struct IoMsg {
  IoMsg() = delete;
  IoMsg(int hdl, int mbl)
      :
      HEADER_LENGTH(hdl),
      MAX_BODY_LENGTH(mbl),
      body_len(0),
      data(std::vector<char>(HEADER_LENGTH + MAX_BODY_LENGTH)){
  }
  ;
  std::size_t length()
  {
    return HEADER_LENGTH + body_len;
  }
  char* body()
  {
    return data.data() + HEADER_LENGTH;;
  }
  const std::size_t HEADER_LENGTH;
  const std::size_t MAX_BODY_LENGTH;
  std::size_t body_len;
  std::vector<char> data;
};

#endif
