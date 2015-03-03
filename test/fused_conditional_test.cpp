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

void canUseConditionalWithLambda() {
  auto const f = [](int i) { return i + 2; };
  auto const g = [](int i, int j) { return 2 * i + j; };
  auto const h = [](int i, int j, int k) { return i * j + k; };
  auto const c = fused::make_conditional(f);
  auto const d = fused::make_conditional(f, g);
  auto const e = fused::make_conditional(f, g, h);
  ASSERT_EQ(f(1), c(1));
  ASSERT_EQ(f(3), c(3));
  ASSERT_EQ(f(1), d(1));
  ASSERT_EQ(f(3), d(3));
  ASSERT_EQ(g(1, 1), d(1, 1));
  ASSERT_EQ(g(3, 2), d(3, 2));
  ASSERT_EQ(f(1), e(1));
  ASSERT_EQ(f(3), e(3));
  ASSERT_EQ(g(1, 1), e(1, 1));
  ASSERT_EQ(g(3, 2), e(3, 2));
  ASSERT_EQ(h(1, 1, 2), e(1, 1, 2));
  ASSERT_EQ(h(3, 2, 3), e(3, 2, 3));
}

int main() {
  canUseConditionalT();
  canUseConditional();
  canUseMakeConditional();
  canUseConditionalWithLambda();
  return 0;
}
