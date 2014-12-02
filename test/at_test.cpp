#include <gtest/gtest.h>
#include <archie/utils/meta/at.h>
#include <type_traits>

namespace {
namespace au = archie::utils;
struct at_test : ::testing::Test {};

TEST_F(at_test, canUseAt) {
  static_assert(
      std::is_same<int, au::meta::at<1>::apply<char, int, float>::type>::value,
      "");
  static_assert(std::is_same<int, au::meta::at_t<1, char, int, float>>::value,
                "");
}

TEST_F(at_test, canUseAtMatch) {
  char a = 0;
  int b = 1;
  float c = 2.0;

  auto& x = au::meta::at<0>::match(a, b, c);
  auto& y = au::meta::at<1>::match(a, b, c);
  auto& z = au::meta::at<2>::match(a, b, c);

  EXPECT_EQ(a, x);
  EXPECT_EQ(b, y);
  EXPECT_EQ(c, z);

  EXPECT_EQ(&a, &x);
  EXPECT_EQ(&b, &y);
  EXPECT_EQ(&c, &z);
}
}
