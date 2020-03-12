#ifndef ABSTRACTIOMSG_HPP
#define ABSTRACTIOMSG_HPP

#include <array>
#include <deque>
#include <algorithm>
#include <iostream>

template<typename Enumeration>
constexpr auto as_integer(
    Enumeration const value) -> typename std::underlying_type<Enumeration>::type {
  static_assert(std::is_enum<Enumeration>::value, "parameter is not of type enum or enum class");
  return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}
class AbstractIoMsg {
 public:
  virtual const char* const_data() const = 0;
  virtual char* data() = 0;
  virtual std::size_t length() const = 0;
  virtual const char* body() const = 0;
  virtual char* body() = 0;
  virtual std::size_t body_length() const = 0;
  virtual void body_length(std::size_t new_length) = 0;
  virtual ~AbstractIoMsg() = default;
};

#endif
