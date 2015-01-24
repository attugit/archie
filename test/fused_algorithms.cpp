#include <archie/utils/fused/tuple.h>
#include <archie/utils/fused/apply.h>
#include <archie/utils/fused/front.h>
#include <archie/utils/fused/back.h>
#include <archie/utils/fused/for_each.h>
#include <archie/utils/test.h>

namespace fused = archie::utils::fused;

void canComposeFusedFront() {
  auto a = fused::apply(fused::front, fused::make_tuple(1, 2u, '3'));
  EXPECT_EQ(1, a);
  auto const& b = fused::apply(fused::front, a, 2u, '3');
  EXPECT_EQ(1, b);
  EXPECT_EQ(&a, &b);
}

void canComposeFusedBack() {
  auto a = fused::apply(fused::back, fused::make_tuple(1, 2u, '3'));
  EXPECT_EQ('3', a);
  auto const& b = fused::apply(fused::back, 1, 2u, a);
  EXPECT_EQ('3', b);
  EXPECT_EQ(&a, &b);
}

void canComposeFusedForEach() {
  auto i = 0u;
  auto f = [&i](auto&&) { ++i; };
  fused::apply(fused::for_each, fused::make_tuple(f, 2u, '3'));
  fused::apply(fused::for_each, f, 2u, '3');
  EXPECT_EQ(4, i);
}

int main() {
  canComposeFusedFront();
  canComposeFusedBack();
  canComposeFusedForEach(); // TODO: kills gdb
  return 0;
}
