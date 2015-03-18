#include <archie/utils/meta/variable_template.h>
#include <archie/utils/fused/number.h>
#include <archie/utils/test.h>

namespace fused = archie::utils::fused;

void canUseFusedNumber() {
  ASSERT_EQ(0u, VARTEMPL(fused::number, 0));
  ASSERT_EQ(1u, VARTEMPL(fused::number, 1));
  ASSERT_EQ(2u, VARTEMPL(fused::number, 2));
}

int main() {
  canUseFusedNumber();
  return 0;
}
