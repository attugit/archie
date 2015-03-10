#include <archie/utils/fused/accumulate.h>
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

int main() {
  canUseAccumulate();
  canApplyAccumulate();
  return 0;
}
