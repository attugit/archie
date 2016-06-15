#include <archie/fused/nth.hpp>
#include <memory>
#include <gtest/gtest.h>

namespace
{
  namespace fused = archie::fused;

  TEST(nth, canUseNth)
  {
    char a = 0;
    int b = 1;
    float c = 2.0;

    auto& x = fused::nth<0>(a, b, c);
    auto& y = fused::nth<1>(a, b, c);
    auto& z = fused::nth<2>(a, b, c);
    EXPECT_EQ(&a, &x);
    EXPECT_EQ(&b, &y);
    EXPECT_EQ(&c, &z);

    fused::nth<0>(a, b, c) = 7;
    EXPECT_EQ(7, a);
  }

  TEST(nth, canUseConstNth)
  {
    const char a = 0;
    const int b = 1;
    const float c = 2.0;

    auto const& x = fused::nth<0>(a, b, c);
    EXPECT_EQ(&a, &x);
  }

  TEST(nth, canWrapNthWithLambda)
  {
    const char a = 0;
    const int b = 1;
    const float c = 2.0;

    auto f = [](auto&&... args) -> decltype(auto) {
      return fused::nth<0>(std::forward<decltype(args)>(args)...);
    };
    auto const& x = f(a, b, c);
    EXPECT_EQ(&a, &x);
  }

  TEST(nth, canUseNthWithNonCopyable)
  {
    auto a = std::make_unique<unsigned>(1u);
    auto b = std::make_unique<int>(2);
    auto c = std::make_unique<char>('3');

    auto const& x = fused::nth<0>(a, b, c);
    auto const& y = fused::nth<1>(a, b, c);
    auto const& z = fused::nth<2>(a, b, c);

    EXPECT_EQ(*a, *x);
    EXPECT_EQ(*b, *y);
    EXPECT_EQ(*c, *z);
  }

  TEST(nth, canUseNthWithRValues)
  {
    auto x = fused::nth<0>(1u, 2.0, '3');
    auto y = fused::nth<1>(1u, 2.0, '3');
    auto z = fused::nth<2>(1u, 2.0, '3');

    EXPECT_EQ(1u, x);
    EXPECT_EQ(2.0, y);
    EXPECT_EQ('3', z);
  }
}
