#include <archie/utils/fused/tuple.h>
#include <archie/utils/fused/apply.h>
#include <archie/utils/fused/front.h>
#include <archie/utils/fused/back.h>
#include <archie/utils/fused/for_each.h>
#include <archie/utils/fused/for_each_order.h>
#include <archie/utils/fused/transform.h>
#include <archie/utils/test.h>
#include <config.h>

#if defined(__clang__)
#include <archie/utils/fused/find.h>
#include <archie/utils/fused/take.h>
#include <archie/utils/fused/nth.h>
#include <archie/utils/fused/type_tag.h>
#endif

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

void canComposeFusedForEachOrder() {
  auto i = 0u;
  auto f = [&i](auto&& x) {
    i *= 10;
    i += x;
  };
  fused::for_each_forward(f, 1u, 2u, 3u);
  EXPECT_EQ(123u, i);
  i = 0u;
  fused::for_each_backward(f, 1u, 2u, 3u);
  EXPECT_EQ(321u, i);
}

void canComposeFusedTransform() {
  auto f = [](auto&& x) { return ++x; };
  auto x = fused::apply(fused::transform, f, 1, 2u, '3');
  static_assert(fused::tuple_size<decltype(x)>::value == 3u, "");
  EXPECT_EQ(2, fused::get<0>(x));
  EXPECT_EQ(3u, fused::get<1>(x));
  EXPECT_EQ('4', fused::get<2>(x));
}

#if defined(__clang__)
void canComposeFusedFind() {
  auto x = fused::apply(fused::find<unsigned>, 1, 2u, '3', 4u);
  EXPECT_EQ(2u, x);
  auto y = fused::apply(fused::find<char>, 1, 2u, '3', 4u);
  EXPECT_EQ('3', y);
}

void canComposeFusedFindIf() {
  auto x = fused::apply(fused::find_if<std::is_unsigned>, 1, 2u, '3', 4u);
  EXPECT_EQ(2u, x);
  auto y = fused::apply(fused::find_if<std::is_signed>, 1, 2u, '3', 4u);
  EXPECT_EQ(1, y);
}

void canComposeFusedTake() {
  auto x = fused::apply(fused::take<2>, fused::make_tuple(1, 2u, '3'));
  static_assert(fused::tuple_size<decltype(x)>::value == 2u, "");
  EXPECT_EQ(1, fused::get<0>(x));
  EXPECT_EQ(2u, fused::get<1>(x));

  auto y = fused::apply(fused::take<3>, 4.0, '3', 2u, 1);
  static_assert(fused::tuple_size<decltype(y)>::value == 3u, "");
  EXPECT_EQ(4.0, fused::get<0>(y));
  EXPECT_EQ('3', fused::get<1>(y));
  EXPECT_EQ(2u, fused::get<2>(y));
}

void canComposeFusedNth() {
  auto x = fused::apply(fused::nth<0>, 1, 2u, '3');
  auto y = fused::apply(fused::nth<1>, 1, 2u, '3');
  auto z = fused::apply(fused::nth<2>, 1, 2u, '3');
  EXPECT_EQ(1, x);
  EXPECT_EQ(2u, y);
  EXPECT_EQ('3', z);
}

void canComposeFusedConstruct() {
  auto x = fused::apply(fused::construct<fused::tuple<int, unsigned, char>>, 1,
                        2u, '3');
  static_assert(fused::tuple_size<decltype(x)>::value == 3u, "");
  EXPECT_EQ(1, fused::get<0>(x));
  EXPECT_EQ(2u, fused::get<1>(x));
  EXPECT_EQ('3', fused::get<2>(x));
}
#endif

int main() {
  canComposeFusedFront();
  canComposeFusedBack();
  canComposeFusedForEach(); // TODO: kills gdb
  canComposeFusedForEachOrder();
  canComposeFusedTransform(); // TODO: kills gdb
#if defined(__clang__)
  canComposeFusedFind();
  canComposeFusedFindIf();
  canComposeFusedTake();
  canComposeFusedNth();
  canComposeFusedConstruct();
#endif
  return 0;
}
