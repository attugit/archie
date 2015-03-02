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
  using f_t = std::remove_reference_t<decltype(f)>;
  using g_t = std::remove_reference_t<decltype(g)>;
  fused::conditional_t<f_t> c{f};
  ASSERT_EQ(3, c(1));
  ASSERT_EQ(5, c(3));
  fused::conditional_t<f_t, g_t> d{f, g};
  ASSERT_EQ(3, d(1));
  ASSERT_EQ(5, d(3));
  ASSERT_EQ(3, d(1, 1));
  ASSERT_EQ(8, d(3, 2));
}

int main() {
  canUseConditionalT();
  canUseConditional();
  canUseMakeConditional();
  canUseConditionalWithLambda();
  return 0;
}
