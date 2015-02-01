#include <archie/utils/fused/tuple.h>
#include <archie/utils/fused/compose.h>
#include <archie/utils/fused/apply.h>
#include <archie/utils/fused/front.h>
#include <archie/utils/fused/back.h>
#include <archie/utils/fused/for_each.h>
#include <archie/utils/fused/for_each_order.h>
#include <archie/utils/fused/transform.h>
#include <archie/utils/fused/concat.h>
#include <archie/utils/fused/zip.h>
#include <archie/utils/fused/tail.h>
#include <archie/utils/test.h>
#include <config.h>

#if defined(HAS_VARIABLE_TEMPLATES)
#include <archie/utils/fused/find.h>
#include <archie/utils/fused/take.h>
#include <archie/utils/fused/nth.h>
#include <archie/utils/fused/type_tag.h>
#include <archie/utils/fused/index_of.h>
#endif

namespace fused = archie::utils::fused;

void canUseFusedCompose() {
  {
    auto x = fused::compose(fused::make_tuple, 1, 2u, '3');
    static_assert(fused::tuple_size<decltype(x)>::value == 3u, "");
    EXPECT_EQ(1, fused::get<0>(x));
    EXPECT_EQ(2u, fused::get<1>(x));
    EXPECT_EQ('3', fused::get<2>(x));
  }
  {
    auto x = fused::compose(fused::make_tuple(fused::front, fused::make_tuple),
                            1, 2u, '3');
    EXPECT_EQ(1, x);
  }
}

void canComposeFusedMakeTuple() {
  auto x = fused::apply(fused::make_tuple, 1, 2u, '3');
  static_assert(fused::tuple_size<decltype(x)>::value == 3u, "");
  EXPECT_EQ(1, fused::get<0>(x));
  EXPECT_EQ(2u, fused::get<1>(x));
  EXPECT_EQ('3', fused::get<2>(x));
}

void canComposeFusedTie() {
  auto a = 1;
  auto b = 2u;
  auto c = '3';
  auto x = fused::apply(fused::tie, a, b, c);
  static_assert(fused::tuple_size<decltype(x)>::value == 3u, "");
  EXPECT_EQ(&a, &fused::get<0>(x));
  EXPECT_EQ(&b, &fused::get<1>(x));
  EXPECT_EQ(&c, &fused::get<2>(x));
}

void canComposeFusedFront() {
  {
    auto a = fused::apply(fused::front, fused::make_tuple(1, 2u, '3'));
    EXPECT_EQ(1, a);
    auto const& b = fused::apply(fused::front, a, 2u, '3');
    EXPECT_EQ(1, b);
    EXPECT_EQ(&a, &b);
  }
  {
    //    auto a = fused::compose(fused::front, fused::make_tuple, 1, 2u, '3');
    //    auto b = 2u;
    //    auto c = '3';

    //    auto const& y = fused::compose(fused::front, fused::tie, a, b, c);
    //    EXPECT_EQ(&a, &y);
  }
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

void canComposeFusedConcat() {
  auto x = fused::apply(fused::concat, fused::make_tuple(1, 2u, '3'), 4.0);
  static_assert(fused::tuple_size<decltype(x)>::value == 4u, "");
  EXPECT_EQ(1, fused::get<0>(x));
  EXPECT_EQ(2u, fused::get<1>(x));
  EXPECT_EQ('3', fused::get<2>(x));
  EXPECT_EQ(4.0, fused::get<3>(x));

  auto y = fused::apply(fused::concat, fused::make_tuple(1, 2u, '3'),
                        fused::make_tuple(4.0, 5, 6u));
  static_assert(fused::tuple_size<decltype(y)>::value == 6u, "");
  EXPECT_EQ(1, fused::get<0>(y));
  EXPECT_EQ(2u, fused::get<1>(y));
  EXPECT_EQ('3', fused::get<2>(y));
  EXPECT_EQ(4.0, fused::get<3>(y));
  EXPECT_EQ(5, fused::get<4>(y));
  EXPECT_EQ(6u, fused::get<5>(y));
}

void canComposeFusedZip() {
  auto x = fused::apply(fused::concat, fused::make_tuple(1, 2u, '3'),
                        fused::make_tuple(4.0, 5, 6u));
  static_assert(fused::tuple_size<decltype(x)>::value == 6u, "");
  EXPECT_EQ(1, fused::get<0>(x));
  EXPECT_EQ(2u, fused::get<1>(x));
  EXPECT_EQ('3', fused::get<2>(x));
  EXPECT_EQ(4.0, fused::get<3>(x));
  EXPECT_EQ(5, fused::get<4>(x));
  EXPECT_EQ(6u, fused::get<5>(x));
}

void canComposeFusedTail() {
  auto x = fused::tail(fused::make_tuple(1, 2u, '3'));
  static_assert(fused::tuple_size<decltype(x)>::value == 2u, "");
  EXPECT_EQ(2u, fused::get<0>(x));
  EXPECT_EQ('3', fused::get<1>(x));
}

#if defined(HAS_VARIABLE_TEMPLATES)
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

void canComposeIndexOf() {
  {
    auto x = fused::apply(fused::index_of<int>, 1, 2u, '3');
    auto y = fused::apply(fused::index_of<unsigned>, 1, 2u, '3');
    auto z = fused::apply(fused::index_of<char>, 1, 2u, '3');
    EXPECT_EQ(0, x);
    EXPECT_EQ(1, y);
    EXPECT_EQ(2, z);
  }
  {
    auto x = fused::apply(fused::index_of<int>, 1);
    EXPECT_EQ(0, x);
  }
  {
    auto tl = fused::type_list<int, unsigned, char>;
    auto x = fused::apply(fused::index_of<int>, tl);
    auto y = fused::apply(fused::index_of<unsigned>, tl);
    auto z = fused::apply(fused::index_of<char>, tl);
    EXPECT_EQ(0, x);
    EXPECT_EQ(1, y);
    EXPECT_EQ(2, z);
  }
}
#endif

int main() {
  canComposeFusedMakeTuple();
  canComposeFusedTie();
  canComposeFusedFront();
  canComposeFusedBack();
  canComposeFusedForEach(); // TODO: kills gdb
  canComposeFusedForEachOrder();
  canComposeFusedTransform(); // TODO: kills gdb
  canComposeFusedConcat();
  canComposeFusedZip();
  canComposeFusedTail();
#if defined(HAS_VARIABLE_TEMPLATES)
  canComposeFusedFind();
  canComposeFusedFindIf();
  canComposeFusedTake();
  canComposeFusedNth();
  canComposeFusedConstruct();
  canComposeIndexOf();
#endif
  return 0;
}
