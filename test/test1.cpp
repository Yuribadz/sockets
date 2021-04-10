#include <gtest/gtest.h>  // googletest header file

#include <algorithm>
#include <iomsg.hpp>
#include <numeric>
#include <vector>

TEST(iomsg, success_write) {
  IoMsg msg(4, 512);
  msg.body_len = 512;
  std::vector<char> testData(512);
  int pv = 0;
  std::generate(testData.begin(), testData.end(), [&pv]() {
    return ++pv;
  });
  msg.data = std::move(testData);
  ASSERT_EQ(msg.body_len, 512);
  ASSERT_EQ(msg.length(), 516);
  ASSERT_EQ(msg.data.size(), 512);
}