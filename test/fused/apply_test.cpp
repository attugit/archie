#include <archie/fused/apply.hpp>
#include <archie/fused/front.hpp>
#include <gtest/gtest.h>

namespace
{
  auto count = [](auto const&... xs) { return sizeof...(xs); };

  namespace fused = archie::fused;

  TEST(apply, canUseApplyWithRegularArgs)
  {
    EXPECT_EQ(1u, fused::apply(count, 3));
    EXPECT_EQ(2u, fused::apply(count, 3, 2));
    EXPECT_EQ(3u, fused::apply(count, 3, 2, 1));
  }

  TEST(apply, canUseApplyWithTuple)
  {
    auto t1 = fused::make_tuple(3);
    auto t2 = fused::make_tuple(3, 2);
    auto t3 = fused::make_tuple(3, 2, 1);

    EXPECT_EQ(1u, fused::apply(count, t1));
    EXPECT_EQ(2u, fused::apply(count, t2));
    EXPECT_EQ(3u, fused::apply(count, t3));
  }

  TEST(apply, canUseApplyWithRValueTuple)
  {
    EXPECT_EQ(1u, fused::apply(count, fused::make_tuple(3)));
    EXPECT_EQ(2u, fused::apply(count, fused::make_tuple(3, 2)));
    EXPECT_EQ(3u, fused::apply(count, fused::make_tuple(3, 2, 1)));
  }

  TEST(apply, canUseApplyWithMakeTuple)
  {
    auto x = fused::apply(fused::make_tuple, 1, 2u, '3');
    static_assert(fused::tuple_size(fused::id<decltype(x)>) == 3u, "");
  }

  TEST(apply, canUseApplyResultOfAnotherApply)
  {
    {
      auto x = fused::apply(fused::front, fused::apply(fused::make_tuple, 1, 2u, '3'));
      EXPECT_EQ(1, x);
    }
    auto a = 1;
    auto b = 2u;
    auto c = '3';
    {
      auto x = fused::apply(fused::front, fused::apply(fused::make_tuple, a, b, c));
      EXPECT_EQ(a, x);
    }
    {
      auto const& x = fused::apply(fused::front, fused::apply(fused::tie, a, b, c));
      EXPECT_EQ(&a, &x);
    }
  }
}
