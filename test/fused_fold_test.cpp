#include <archie/utils/fused/fold.h>
#include <archie/utils/fused/tuple.h>
#include <archie/utils/fused/apply.h>
#include <archie/utils/test.h>

namespace fused = archie::utils::fused;

void canUseFold() {
  auto const mkt = [](auto&& s, auto&& e) { return fused::make_tuple(s, e); };
  EXPECT_EQ(fused::make_tuple(0, 1), fused::fold(mkt, 0, 1));
  EXPECT_EQ(fused::make_tuple(fused::make_tuple(0, 1), 2), fused::fold(mkt, 0, 1, 2));
}

int main() {
  canUseFold();
  return 0;
}
