#include <archie/utils/fused/front.h>
#include <test/assert.h>

namespace fused = archie::utils::fused;

void canUseFrontWithRValue() {
  auto a = fused::front(1u, 2.0, '3');
  EXPECT_EQ(1u, a);
}

void canUseBackWithLValue() {
  unsigned a = 1u;
  double b = 2.0;
  char c = '3';
  auto const& x = fused::front(a, b, c);
  EXPECT_EQ(1u, x);
  EXPECT_EQ(&a, &x);
}

int main() {
  canUseFrontWithRValue();
  canUseBackWithLValue();
  return 0;
}
