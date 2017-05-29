#include <archie/fused/overload.hpp>
#include <gtest/gtest.h>

namespace
{
  struct goo {
    int operator()() const { return 3; }
  };
  struct hoo {
    int operator()(int) const { return 4; }
  };

  namespace fused = archie::fused;

  TEST(overload, canUseConditionalT)
  {
    fused::overload<goo, hoo> cond;
    auto x = cond();
    auto y = cond(1);
    EXPECT_EQ(3, x);
    EXPECT_EQ(4, y);
  }

  TEST(overload, canUseConditional)
  {
    auto x = fused::overload<goo, hoo>()();
    auto y = fused::overload<goo, hoo>()(1);
    EXPECT_EQ(3, x);
    EXPECT_EQ(4, y);
  }

  TEST(overload, canUseMakeConditional)
  {
    constexpr auto f = fused::make_overload(goo{}, hoo{});
    EXPECT_EQ(3, f());
    EXPECT_EQ(4, f(7));
  }

  TEST(overload, canUseConditionalWithLambda)
  {
    auto const f = [](int i) { return i + 2; };
    auto const g = [](int i, int j) { return 2 * i + j; };
    auto const h = [](int i, int j, int k) { return i * j + k; };
    auto const c = fused::make_overload(f);
    auto const d = fused::make_overload(f, g);
    auto const e = fused::make_overload(f, g, h);
    EXPECT_EQ(f(1), c(1));
    EXPECT_EQ(f(3), c(3));
    EXPECT_EQ(f(1), d(1));
    EXPECT_EQ(f(3), d(3));
    EXPECT_EQ(g(1, 1), d(1, 1));
    EXPECT_EQ(g(3, 2), d(3, 2));
    EXPECT_EQ(f(1), e(1));
    EXPECT_EQ(f(3), e(3));
    EXPECT_EQ(g(1, 1), e(1, 1));
    EXPECT_EQ(g(3, 2), e(3, 2));
    EXPECT_EQ(h(1, 1, 2), e(1, 1, 2));
    EXPECT_EQ(h(3, 2, 3), e(3, 2, 3));
  }
}
