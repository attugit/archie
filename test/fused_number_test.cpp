#include <archie/utils/fused/number.h>
#include <archie/utils/test.h>

namespace fused = archie::utils::fused;

void canUseFusedNumber() {
  ASSERT_EQ(0u, fused::number<0>);
  ASSERT_EQ(1u, fused::number<1>);
  ASSERT_EQ(2u, fused::number<2>);
}

int main() {
  canUseFusedNumber();
  return 0;
}
