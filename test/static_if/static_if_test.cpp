#include <archie/fused/static_if.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <string>

namespace {
using namespace archie::fused;
TEST(static_if, canUseStaticIf)
{
  auto const size1 = static_if(std::true_type{})([](std::vector<int> v) { return v.size(); },
                                                 [](std::string s) { return s.size(); })({1, 2, 3});
  EXPECT_EQ(size1, 3u);

  auto const size2 = static_if(std::false_type{})([](std::vector<int> v) { return v.size(); },
                                                  [](std::string s) { return s.size(); })("Hello");
  EXPECT_EQ(size2, 5u);

  std::vector<int> v;
  static_if(std::true_type{})([](std::vector<int>& vv) { vv.reserve(3); })(v);
  EXPECT_EQ(v.capacity(), 3u);
  static_if(std::false_type{})([](std::vector<int>& vv) { vv.reserve(7); })(v);
  EXPECT_EQ(v.capacity(), 3u);
}
}
