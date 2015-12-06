#include <archie/test.h>
#include <archie/fused/back.h>

namespace meta = archie::meta;

template <typename Tp>
decltype(auto) second(meta::ignore, Tp&& t, ...) {
  return std::forward<Tp>(t);
}

void canUseEatToTakeSecondArgument() {
  auto sec = second(1u, 2.0, '3');
  EXPECT_EQ(2.0, sec);
}

namespace fused = archie::fused;

#include <memory>

void canUseBackWithRValue() {
  auto a = fused::back(std::make_unique<unsigned>(1u), 2.0, '3');
  EXPECT_EQ('3', a);
}

void canUseBackWithLValue() {
  auto a = std::make_unique<unsigned>(1u);
  double b = 2.0;
  char c = '3';
  auto const& x = fused::back(a, b, c);
  EXPECT_EQ('3', x);
  EXPECT_EQ(&c, &x);
}

int main() {
  canUseEatToTakeSecondArgument();
  canUseBackWithRValue();
  canUseBackWithLValue();
  return 0;
}
