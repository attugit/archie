#include <archie/utils/fused/placeholder.h>
namespace fused = archie::utils::fused;
#include <archie/utils/test.h>

using namespace fused::placeholders;

void canAccessArgumentWithPlaceholder() {
  auto a = 1u;
  auto b = 2.0;
  auto c = '3';

  auto const& x = _0(a, b, c);
  auto const& y = _1(a, b, c);
  auto const& z = _2(a, b, c);

  EXPECT_EQ(&a, &x);
  EXPECT_EQ(&b, &y);
  EXPECT_EQ(&c, &z);
}

void canBindPlaceHolders() {
  auto inc = [](auto&& x, auto&& y, auto&& z) {
    x += 1;
    y += 2;
    z += 3;
  };

  auto func = fused::reorder(inc, _1, _2, _0);

  auto a = 1u;
  auto b = 2.0;
  auto c = '3';

  func(a, b, c);

  EXPECT_EQ(4u, a);
  EXPECT_EQ(3.0, b);
  EXPECT_EQ('5', c);
}

int main() {
  canAccessArgumentWithPlaceholder();
  canBindPlaceHolders();
  return 0;
}
