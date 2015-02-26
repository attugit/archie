#include <archie/utils/fused/boolean.h>
#include <archie/utils/test.h>

namespace fused = archie::utils::fused;

void canUseFusedBoolean() {
  ASSERT_TRUE(fused::True);
  ASSERT_FALSE(fused::False);
}

int main() {
  canUseFusedBoolean();
  return 0;
}
