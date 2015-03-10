#include <archie/utils/fused/fold_algorithms.h>
#include <archie/utils/fused/tuple.h>
#include <archie/utils/fused/apply.h>
#include <archie/utils/test.h>

namespace fused = archie::utils::fused;

void canUseAccumulate() {
  EXPECT_EQ(1, fused::accumulate(0, 1));
  EXPECT_EQ(3, fused::accumulate(0, 1, 2));
  EXPECT_EQ(6, fused::accumulate(0, 1, 2, 3));
}

void canApplyAccumulate() {
  auto const t = fused::make_tuple(0, 1, 2);
  EXPECT_EQ(3, fused::apply(fused::accumulate, t));
  EXPECT_EQ(6, fused::apply(fused::accumulate, fused::make_tuple(0, 1, 2, 3)));
}

void canUseAccumulateWithCustomFunctionObject() {
  auto const t = fused::make_tuple(0, 1, 2);
  auto const f = [](auto a, auto b) { return a + 3 * b; };
  EXPECT_EQ(9, fused::apply(fused::make_fold(f), t));
  EXPECT_EQ(9, fused::apply(fused::make_fold(f, 0), t));
  EXPECT_EQ(10, fused::apply(fused::make_fold(f, 1), t));
}

void canUseMax() {
  EXPECT_EQ(1, fused::max(0, 1));
  EXPECT_EQ(2, fused::max(0, 1, 2));
  EXPECT_EQ(3, fused::max(0, 3, 2));
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
  return 0;
}
