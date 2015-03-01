#include <archie/utils/fused/conditional.h>
#include <archie/utils/test.h>

struct goo {
  int operator()() const { return 3; }
};
struct hoo {
  int operator()(int) const { return 4; }
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

void canUseMakeConditional() {
  constexpr auto f = fused::make_conditional(goo{}, hoo{});
  ASSERT_EQ(3, f());
  ASSERT_EQ(4, f(7));
}

int main() {
  canUseConditionalT();
  canUseConditional();
  canUseMakeConditional();
  return 0;
}
