#include <gtest/gtest.h>
#include <archie/utils/fused/nth.h>

namespace {
using archie::utils::fused::nth;
struct nth_test : ::testing::Test {};

TEST_F(nth_test, canUseNth) {
  char a = 0;
  int b = 1;
  float c = 2.0;

  auto& x = nth<0>(a, b, c);
  auto& y = nth<1>(a, b, c);
  auto& z = nth<2>(a, b, c);
  EXPECT_EQ(&a, &x);
  EXPECT_EQ(&b, &y);
  EXPECT_EQ(&c, &z);

  nth<0>(a, b, c) = 7;
  EXPECT_EQ(7, a);
}

TEST_F(nth_test, canUseConstNth) {
  const char a = 0;
  const int b = 1;
  const float c = 2.0;

  auto const& x = nth<0>(a, b, c);
  EXPECT_EQ(&a, &x);
}

TEST_F(nth_test, canWrapNthWithLambda) {
  const char a = 0;
  const int b = 1;
  const float c = 2.0;

  auto f = [](auto&&... args) -> decltype(auto) {
    return nth<0>(std::forward<decltype(args)>(args)...);
  };
  auto const& x = f(a, b, c);
  EXPECT_EQ(&a, &x);
}
}
