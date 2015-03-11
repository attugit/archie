#include <archie/utils/fused/fold_algorithms.h>
#include <archie/utils/fused/tuple.h>
#include <archie/utils/fused/apply.h>
#include <archie/utils/test.h>
#include <functional>

namespace fused = archie::utils::fused;

void canUseAccumulate() {
  auto const plus = std::plus<>{};
  EXPECT_EQ(1u, fused::accumulate(plus, 0, 1u));
  EXPECT_EQ(3.0, fused::accumulate(plus, 0, 1u, 2.0));
  EXPECT_EQ(54.0, fused::accumulate(plus, 0, 1u, 2.0, '3'));
}

void canApplyAccumulate() {
  auto const plus = std::plus<>{};
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
  EXPECT_EQ(1.0, fused::max(0, 1.0));
  EXPECT_EQ('1', fused::max(0, '1', 2.0));
  EXPECT_EQ(3, fused::max(0, 3, 2.0));
}

void canApplyMax() {
  auto const t = fused::make_tuple(0, 3, 2);
  EXPECT_EQ(3, fused::apply(fused::max, t));
  EXPECT_EQ(4, fused::apply(fused::max, fused::make_tuple(4, 3, 2)));
}

void canApplyMaxWithCustomFunctionObject() {
  auto const t = fused::make_tuple(0, 3, 4);
  auto const f = [](auto a, auto b) { return b > (2 * a) ? b : a; };
  EXPECT_EQ(3, fused::apply(fused::make_fold(f), t));
  EXPECT_EQ(4, fused::apply(fused::make_fold(f), fused::make_tuple(4, 3, 6)));
}

void canUseMin() {
  EXPECT_EQ(0, fused::min(0, 1));
  EXPECT_EQ(0, fused::min(2, 0, 1));
  EXPECT_EQ(0, fused::min(3, 2, 0));
}

void canApplyMin() {
  auto const t = fused::make_tuple(3, 0, 2);
  EXPECT_EQ(0, fused::apply(fused::min, t));
  EXPECT_EQ(0, fused::apply(fused::min, fused::make_tuple(0, 3, 2)));
}

void canApplyMinWithCustomFunctionObject() {
  auto const t = fused::make_tuple(4, 1, 0);
  auto const f = [](auto a, auto b) { return b < (a - 2) ? b : a; };
  EXPECT_EQ(1, fused::apply(fused::make_fold(f), t));
  EXPECT_EQ(4, fused::apply(fused::make_fold(f), fused::make_tuple(4, 3, 2)));
}

void canUseAllOf() {
  auto const is_odd = [](auto x) { return x % 2 != 0; };
  EXPECT_TRUE(fused::all_of(is_odd, 1, 3, 5));
  EXPECT_FALSE(fused::all_of(is_odd, 1, 2, 5));
  EXPECT_FALSE(fused::all_of(is_odd, 2, 2, 2));
}

void canApplyAllOf() {
  auto const is_odd = [](auto x) { return x % 2 != 0; };
  auto const odds = fused::make_tuple(1, 3, 5);
  auto const mixed = fused::make_tuple(1, 2, 3);
  auto const evens = fused::make_tuple(2, 4, 6);
  EXPECT_TRUE(fused::apply(fused::all_of(is_odd), odds));
  EXPECT_FALSE(fused::apply(fused::all_of(is_odd), mixed));
  EXPECT_FALSE(fused::apply(fused::all_of(is_odd), evens));
}

void canUseAnyOf() {
  auto const is_odd = [](auto x) { return x % 2 != 0; };
  EXPECT_TRUE(fused::any_of(is_odd, 1, 3, 5));
  EXPECT_TRUE(fused::any_of(is_odd, 1, 2, 5));
  EXPECT_FALSE(fused::any_of(is_odd, 2, 2, 2));
}

void canApplyAnyOf() {
  auto const is_odd = [](auto x) { return x % 2 != 0; };
  auto const odds = fused::make_tuple(1, 3, 5);
  auto const mixed = fused::make_tuple(1, 2, 3);
  auto const evens = fused::make_tuple(2, 4, 6);
  EXPECT_TRUE(fused::apply(fused::any_of(is_odd), odds));
  EXPECT_TRUE(fused::apply(fused::any_of(is_odd), mixed));
  EXPECT_FALSE(fused::apply(fused::any_of(is_odd), evens));
}

void canUseNoneOf() {
  auto const is_odd = [](auto x) { return x % 2 != 0; };
  EXPECT_FALSE(fused::none_of(is_odd, 1, 3, 5));
  EXPECT_FALSE(fused::none_of(is_odd, 1, 2, 5));
  EXPECT_TRUE(fused::none_of(is_odd, 2, 2, 2));
}

void canApplyNoneOf() {
  auto const is_odd = [](auto x) { return x % 2 != 0; };
  auto const odds = fused::make_tuple(1, 3, 5);
  auto const mixed = fused::make_tuple(1, 2, 3);
  auto const evens = fused::make_tuple(2, 4, 6);
  EXPECT_FALSE(fused::apply(fused::none_of(is_odd), odds));
  EXPECT_FALSE(fused::apply(fused::none_of(is_odd), mixed));
  EXPECT_TRUE(fused::apply(fused::none_of(is_odd), evens));
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
  return 0;
}
