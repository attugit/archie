#include <archie/utils/fused/fold_algorithms.h>
#include <archie/utils/fused/tuple.h>
#include <archie/utils/fused/apply.h>
#include <archie/utils/test.h>
#include <functional>

namespace fused = archie::utils::fused;

auto const plus = std::plus<>{};
auto const ge = std::greater_equal<>{};
auto const le = std::less_equal<>{};
auto const is_odd = [](auto x) { return x % 2 != 0; };

void canUseAccumulate() {
  EXPECT_EQ(1u, fused::accumulate(plus)(0, 1u));
  EXPECT_EQ(3.0, fused::accumulate(plus)(0, 1u, 2.0));
  EXPECT_EQ(54.0, fused::accumulate(plus)(0, 1u, 2.0, '3'));
}

void canApplyAccumulate() {
  auto const t = fused::make_tuple(0, 1, 2);
  EXPECT_EQ(3, fused::apply(fused::accumulate(plus), t));
  EXPECT_EQ(6, fused::apply(fused::accumulate(plus), fused::make_tuple(0, 1, 2, 3)));
}

void canUseAccumulateWithCustomFunctionObject() {
  auto const t = fused::make_tuple(0, 1, 2);
  auto const f = [](auto a, auto b) { return a + 3 * b; };
  EXPECT_EQ(9, fused::apply(fused::accumulate(f), t));
  EXPECT_EQ(9, fused::apply(fused::accumulate(f, 0), t));
  EXPECT_EQ(10, fused::apply(fused::accumulate(f, 1), t));
}

void canUseMax() {
  auto const max = fused::extremum(ge);
  EXPECT_EQ(1.0, max(0, 1.0));
  EXPECT_EQ('1', max(0, '1', 2.0));
  EXPECT_EQ(3, max(0, 3, 2.0));
}

void canApplyMax() {
  auto const max = fused::extremum(ge);
  auto const t = fused::make_tuple(0, 3, 2);
  EXPECT_EQ(3, fused::apply(max, t));
  EXPECT_EQ(4, fused::apply(max, fused::make_tuple(4, 3, 2)));
  auto const max5 = fused::extremum(ge, 5);
  EXPECT_EQ(5, fused::apply(max5, fused::make_tuple(4, 3, 2)));
}

void canApplyMaxWithCustomFunctionObject() {
  auto const t = fused::make_tuple(0, 3, 4);
  auto const f = [](auto const& a, auto const& b) { return (2 * a) >= b; };
  auto const max = fused::extremum(f);
  EXPECT_EQ(3, fused::apply(max, t));
  EXPECT_EQ(4, fused::apply(max, fused::make_tuple(4, 3, 6)));
}

void canUseMin() {
  auto const min = fused::extremum(le);
  EXPECT_EQ(0, min(0, 1));
  EXPECT_EQ(0, min(2, 0, 1));
  EXPECT_EQ(0, min(3, 2, 0));
}

void canApplyMin() {
  auto const min = fused::extremum(le);
  auto const t = fused::make_tuple(3, 0, 2);
  EXPECT_EQ(0, fused::apply(min, t));
  EXPECT_EQ(0, fused::apply(min, fused::make_tuple(0, 3, 2)));
  auto const min2 = fused::extremum(le, 2);
  EXPECT_EQ(2, fused::apply(min2, fused::make_tuple(4, 3, 5)));
}

void canApplyMinWithCustomFunctionObject() {
  auto const t = fused::make_tuple(4, 1, 0);
  auto const f = [](auto a, auto b) { return (a - 2) <= b; };
  auto const min = fused::extremum(f);
  EXPECT_EQ(1, fused::apply(min, t));
  EXPECT_EQ(4, fused::apply(min, fused::make_tuple(4, 3, 2)));
}

void canUseAllOf() {
  EXPECT_TRUE(fused::all_of(is_odd)(1, 3, 5));
  EXPECT_FALSE(fused::all_of(is_odd)(1, 2, 5));
  EXPECT_FALSE(fused::all_of(is_odd)(2, 2, 2));
}

void canApplyAllOf() {
  auto const odds = fused::make_tuple(1, 3, 5);
  auto const mixed = fused::make_tuple(1, 2, 3);
  auto const evens = fused::make_tuple(2, 4, 6);
  EXPECT_TRUE(fused::apply(fused::all_of(is_odd), odds));
  EXPECT_FALSE(fused::apply(fused::all_of(is_odd), mixed));
  EXPECT_FALSE(fused::apply(fused::all_of(is_odd), evens));
}

void canUseAnyOf() {
  EXPECT_TRUE(fused::any_of(is_odd)(1, 3, 5));
  EXPECT_TRUE(fused::any_of(is_odd)(1, 2, 5));
  EXPECT_FALSE(fused::any_of(is_odd)(2, 2, 2));
}

void canApplyAnyOf() {
  auto const odds = fused::make_tuple(1, 3, 5);
  auto const mixed = fused::make_tuple(1, 2, 3);
  auto const evens = fused::make_tuple(2, 4, 6);
  EXPECT_TRUE(fused::apply(fused::any_of(is_odd), odds));
  EXPECT_TRUE(fused::apply(fused::any_of(is_odd), mixed));
  EXPECT_FALSE(fused::apply(fused::any_of(is_odd), evens));
}

void canUseNoneOf() {
  EXPECT_FALSE(fused::none_of(is_odd)(1, 3, 5));
  EXPECT_FALSE(fused::none_of(is_odd)(1, 2, 5));
  EXPECT_TRUE(fused::none_of(is_odd)(2, 2, 2));
}

void canApplyNoneOf() {
  auto const odds = fused::make_tuple(1, 3, 5);
  auto const mixed = fused::make_tuple(1, 2, 3);
  auto const evens = fused::make_tuple(2, 4, 6);
  EXPECT_FALSE(fused::apply(fused::none_of(is_odd), odds));
  EXPECT_FALSE(fused::apply(fused::none_of(is_odd), mixed));
  EXPECT_TRUE(fused::apply(fused::none_of(is_odd), evens));
}

void canUseCountIf() {
  EXPECT_EQ(0, fused::count_if(is_odd)(0));
  EXPECT_EQ(1, fused::count_if(is_odd)(1));
  EXPECT_EQ(1, fused::count_if(is_odd)(0, 1, 2));
  EXPECT_EQ(2, fused::count_if(is_odd)(0, 1, 2, 3));
}

void canApplyCountIf() {
  EXPECT_EQ(0, fused::apply(fused::count_if(is_odd), fused::make_tuple(0)));
  EXPECT_EQ(1, fused::apply(fused::count_if(is_odd), fused::make_tuple(1)));
  EXPECT_EQ(1, fused::apply(fused::count_if(is_odd), fused::make_tuple(0, 1, 2)));
  EXPECT_EQ(2, fused::apply(fused::count_if(is_odd), fused::make_tuple(0, 1, 2, 3)));
}

int main() {
  canUseAccumulate();
  canApplyAccumulate();
  canUseAccumulateWithCustomFunctionObject();
  canUseMax();
  canApplyMax();
  canApplyMaxWithCustomFunctionObject();
  canUseMin();
  canApplyMin();
  canApplyMinWithCustomFunctionObject();
  canUseAllOf();
  canApplyAllOf();
  canUseAnyOf();
  canApplyAnyOf();
  canUseNoneOf();
  canApplyNoneOf();
  canUseCountIf();
  canApplyCountIf();
  return 0;
}
