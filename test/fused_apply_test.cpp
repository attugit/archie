#include <archie/utils/fused/apply.h>

auto count = [](auto const&... xs) { return sizeof...(xs); };

namespace fused = archie::utils::fused;

#include <archie/utils/test.h>

void canUseApplyWithRegularArgs() {
  EXPECT_EQ(1u, fused::apply(count, 3));
  EXPECT_EQ(2u, fused::apply(count, 3, 2));
  EXPECT_EQ(3u, fused::apply(count, 3, 2, 1));
}

void canUseApplyWithTuple() {
  auto t1 = fused::make_tuple(3);
  auto t2 = fused::make_tuple(3, 2);
  auto t3 = fused::make_tuple(3, 2, 1);

  EXPECT_EQ(1u, fused::apply(count, t1));
  EXPECT_EQ(2u, fused::apply(count, t2));
  EXPECT_EQ(3u, fused::apply(count, t3));
}

int main() {
  canUseApplyWithRegularArgs();
  canUseApplyWithTuple();
  return 0;
}
