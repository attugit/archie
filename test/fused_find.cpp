#include <archie/utils/fused/find.h>
#include <archie/utils/test.h>

namespace fused = archie::utils::fused;

void canUseFusedFind() {
  unsigned a = 0u;
  int b = 1;
  char c = '2';
  double d = 3.0;
  unsigned e = 4u;

  auto x = fused::find<unsigned&>(a, b, c, d, e);
  auto y = fused::find<double&>(a, b, c, d, e);
  EXPECT_EQ(a, x);
  EXPECT_EQ(d, y);
}

int main() {
  canUseFusedFind();
  return 0;
}
