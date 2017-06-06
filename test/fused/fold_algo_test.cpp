#include <archie/fused/tuple.hpp>
#include <archie/fused/algorithm.hpp>
#include <functional>
#include <gtest/gtest.h>

namespace
{
  namespace fused = archie::fused;

  auto const plus = std::plus<>{};
  auto const ge = std::greater_equal<>{};
  auto const le = std::less_equal<>{};
  auto const is_odd = [](auto x) { return x % 2 != 0; };

  TEST(fold_algo, canUseAccumulate)
  {
    EXPECT_EQ(1u, fused::reduce(plus)(0, 1u));
    EXPECT_EQ(3.0, fused::reduce(plus)(0, 1u, 2.0));
    EXPECT_EQ(54.0, fused::reduce(plus)(0, 1u, 2.0, '3'));
  }

  TEST(fold_algo, canApplyAccumulate)
  {
    auto const t = fused::make_tuple(0, 1, 2);
    EXPECT_EQ(3, fused::apply(fused::reduce(plus), t));
    EXPECT_EQ(6, fused::apply(fused::reduce(plus), fused::make_tuple(0, 1, 2, 3)));
  }

  TEST(fold_algo, canUseAccumulateWithCustomFunctionObject)
  {
    auto const t = fused::make_tuple(0, 1, 2);
    auto const f = [](auto a, auto b) { return a + 3 * b; };
    EXPECT_EQ(9, fused::apply(fused::reduce(f), t));
    EXPECT_EQ(9, fused::apply(fused::reduce(f, 0), t));
    EXPECT_EQ(10, fused::apply(fused::reduce(f, 1), t));
  }

  TEST(fold_algo, canUseMax)
  {
    EXPECT_EQ(1.0, fused::max(0.0, 1.0));
    EXPECT_EQ('2', fused::max('0', '1', '2'));
    EXPECT_EQ(3, fused::max(0, 3, 2));
  }

  TEST(fold_algo, canApplyMax)
  {
    auto const t = fused::make_tuple(0, 3, 2);
    EXPECT_EQ(3, fused::apply(fused::max, t));
    EXPECT_EQ(4, fused::apply(fused::max, fused::make_tuple(4, 3, 2)));
    auto const max5 = fused::extremum(ge, 5);
    EXPECT_EQ(5, fused::apply(max5, fused::make_tuple(4, 3, 2)));
  }

  TEST(fold_algo, canApplyMaxWithCustomFunctionObject)
  {
    auto const t = fused::make_tuple(0, 3, 4);
    auto const f = [](auto const& a, auto const& b) { return (2 * a) >= b; };
    auto const max = fused::extremum(f);
    EXPECT_EQ(3, fused::apply(max, t));
    EXPECT_EQ(4, fused::apply(max, fused::make_tuple(4, 3, 6)));
  }

  TEST(fold_algo, canUseMin)
  {
    EXPECT_EQ(0, fused::min(0, 1));
    EXPECT_EQ(0, fused::min(2, 0, 1));
    EXPECT_EQ(0, fused::min(3, 2, 0));
  }

  TEST(fold_algo, canApplyMin)
  {
    auto const t = fused::make_tuple(3, 0, 2);
    EXPECT_EQ(0, fused::apply(fused::min, t));
    EXPECT_EQ(0, fused::apply(fused::min, fused::make_tuple(0, 3, 2)));
    auto const min2 = fused::extremum(le, 2);
    EXPECT_EQ(2, fused::apply(min2, fused::make_tuple(4, 3, 5)));
  }

  TEST(fold_algo, canApplyMinWithCustomFunctionObject)
  {
    auto const t = fused::make_tuple(4, 1, 0);
    auto const f = [](auto a, auto b) { return (a - 2) <= b; };
    auto const min = fused::extremum(f);
    EXPECT_EQ(1, fused::apply(min, t));
    EXPECT_EQ(4, fused::apply(min, fused::make_tuple(4, 3, 2)));
  }

  TEST(fold_algo, canUseAllOf)
  {
    EXPECT_TRUE(fused::all_of(is_odd)(1, 3, 5));
    EXPECT_FALSE(fused::all_of(is_odd)(1, 2, 5));
    EXPECT_FALSE(fused::all_of(is_odd)(2, 2, 2));
  }

  TEST(fold_algo, canApplyAllOf)
  {
    auto const odds = fused::make_tuple(1, 3, 5);
    auto const mixed = fused::make_tuple(1, 2, 3);
    auto const evens = fused::make_tuple(2, 4, 6);
    EXPECT_TRUE(fused::apply(fused::all_of(is_odd), odds));
    EXPECT_FALSE(fused::apply(fused::all_of(is_odd), mixed));
    EXPECT_FALSE(fused::apply(fused::all_of(is_odd), evens));
  }

  TEST(fold_algo, canUseAnyOf)
  {
    EXPECT_TRUE(fused::any_of(is_odd)(1, 3, 5));
    EXPECT_TRUE(fused::any_of(is_odd)(1, 2, 5));
    EXPECT_FALSE(fused::any_of(is_odd)(2, 2, 2));
  }

  TEST(fold_algo, canApplyAnyOf)
  {
    auto const odds = fused::make_tuple(1, 3, 5);
    auto const mixed = fused::make_tuple(1, 2, 3);
    auto const evens = fused::make_tuple(2, 4, 6);
    EXPECT_TRUE(fused::apply(fused::any_of(is_odd), odds));
    EXPECT_TRUE(fused::apply(fused::any_of(is_odd), mixed));
    EXPECT_FALSE(fused::apply(fused::any_of(is_odd), evens));
  }

  TEST(fold_algo, canUseNoneOf)
  {
    EXPECT_FALSE(fused::none_of(is_odd)(1, 3, 5));
    EXPECT_FALSE(fused::none_of(is_odd)(1, 2, 5));
    EXPECT_TRUE(fused::none_of(is_odd)(2, 2, 2));
  }

  TEST(fold_algo, canApplyNoneOf)
  {
    auto const odds = fused::make_tuple(1, 3, 5);
    auto const mixed = fused::make_tuple(1, 2, 3);
    auto const evens = fused::make_tuple(2, 4, 6);
    EXPECT_FALSE(fused::apply(fused::none_of(is_odd), odds));
    EXPECT_FALSE(fused::apply(fused::none_of(is_odd), mixed));
    EXPECT_TRUE(fused::apply(fused::none_of(is_odd), evens));
  }

  TEST(fold_algo, canUseCountIf)
  {
    EXPECT_EQ(0u, fused::count_if(is_odd)(0));
    EXPECT_EQ(1u, fused::count_if(is_odd)(1));
    EXPECT_EQ(1u, fused::count_if(is_odd)(0, 1, 2));
    EXPECT_EQ(2u, fused::count_if(is_odd)(0, 1, 2, 3));
  }

  TEST(fold_algo, canApplyCountIf)
  {
    EXPECT_EQ(0u, fused::apply(fused::count_if(is_odd), fused::make_tuple(0)));
    EXPECT_EQ(1u, fused::apply(fused::count_if(is_odd), fused::make_tuple(1)));
    EXPECT_EQ(1u, fused::apply(fused::count_if(is_odd), fused::make_tuple(0, 1, 2)));
    EXPECT_EQ(2u, fused::apply(fused::count_if(is_odd), fused::make_tuple(0, 1, 2, 3)));
  }

  TEST(fold_algo, canUseIsSorted)
  {
    EXPECT_TRUE(fused::is_sorted(le)());
    EXPECT_TRUE(fused::is_sorted(le)(0));
    EXPECT_TRUE(fused::is_sorted(le)(0, 1));
    EXPECT_TRUE(fused::is_sorted(le)(0, 1, 2));
    EXPECT_TRUE(fused::is_sorted(le)(0, 1, 2, 3));
    EXPECT_FALSE(fused::is_sorted(le)(0, 1, 2, 0));
    EXPECT_FALSE(fused::is_sorted(le)(0, 1, 0, 3));
  }
  TEST(fold_algo, canApplyIsSorted)
  {
    EXPECT_TRUE(fused::apply(fused::is_sorted(le), fused::make_tuple(0)));
    EXPECT_TRUE(fused::apply(fused::is_sorted(le), fused::make_tuple(0, 1)));
    EXPECT_TRUE(fused::apply(fused::is_sorted(le), fused::make_tuple(0, 1, 2)));
    EXPECT_TRUE(fused::apply(fused::is_sorted(le), fused::make_tuple(0, 1, 2, 3)));
    EXPECT_FALSE(fused::apply(fused::is_sorted(le), fused::make_tuple(0, 1, 2, 0)));
    EXPECT_FALSE(fused::apply(fused::is_sorted(le), fused::make_tuple(0, 1, 0, 3)));
  }
}
