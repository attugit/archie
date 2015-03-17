#include <archie/utils/meta/variable_template.h>
#include <archie/utils/fused/number.h>
#include <archie/utils/test.h>

namespace fused = archie::utils::fused;

void canUseFusedNumber() {
#if !defined(HAS_VARIABLE_TEMPLATES)
  constexpr auto& a = fused::number<0>::value;
  constexpr auto& b = fused::number<1>::value;
  constexpr auto& c = fused::number<2>::value;
#else
  constexpr auto& a = fused::number<0>;
  constexpr auto& b = fused::number<1>;
  constexpr auto& c = fused::number<2>;
#endif
  ASSERT_EQ(0u, a);
  ASSERT_EQ(1u, b);
  ASSERT_EQ(2u, c);
}

int main() {
  canUseFusedNumber();
  return 0;
}
