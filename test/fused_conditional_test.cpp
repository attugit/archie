#include <archie/utils/fused/conditional.h>
#include <archie/utils/test.h>

struct goo {
  int operator()() { return 3; }
};
struct hoo {
  int operator()(int) { return 4; }
};

namespace fused = archie::utils::fused;

void canUseConditionalT() {
  fused::conditional_t<goo, hoo> cond;
  auto x = cond();
  auto y = cond(1);
  ASSERT_EQ(3, x);
  ASSERT_EQ(4, y);
}

void canUseConditional() {
#if defined(HAS_VARIABLE_TEMPLATES)
  constexpr auto cond = fused::conditional<goo, hoo>;
#else
  constexpr auto cond = fused::conditional<goo, hoo>::value;
#endif
  auto x = cond();
  auto y = cond(1);
  ASSERT_EQ(3, x);
  ASSERT_EQ(4, y);
}

int main() {
  canUseConditionalT();
  canUseConditional();
  return 0;
}
