#include <config.h>
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
#include <archie/utils/fused/find.h>
#include <archie/utils/test.h>
#include <archie/utils/fused/take.h>
#include <archie/utils/fused/nth.h>
#include <archie/utils/fused/type_tag.h>
#include <archie/utils/fused/index_of.h>

#include <memory>

namespace fused = archie::utils::fused;

void canUseFusedCompose() {
  {
    auto x = fused::compose(fused::make_tuple, 1, 2u, '3');
    static_assert(fused::tuple_size(fused::type_tag<decltype(x)>{}) == 3u, "");
    EXPECT_EQ(1, fused::get<0>(x));
    EXPECT_EQ(2u, fused::get<1>(x));
    EXPECT_EQ('3', fused::get<2>(x));
  }
  {
    auto a = 1;
    auto b = 2u;
    auto c = '3';

    auto const& x = fused::compose(fused::make_tuple(fused::front, fused::tie), a, b, c);
    EXPECT_EQ(&a, &x);
  }
  {
    auto a = 1;
    auto b = 2u;
    auto c = '3';

    auto opt = fused::make_tuple(fused::front, fused::tie);
    auto const& x = fused::compose(opt, a, b, c);
    EXPECT_EQ(&a, &x);
  }
}

void canComposeFusedMakeTuple() {
  auto x = fused::apply(fused::make_tuple, 1, 2u, '3');
  static_assert(fused::tuple_size(fused::type_tag<decltype(x)>{}) == 3u, "");
  EXPECT_EQ(1, fused::get<0>(x));
  EXPECT_EQ(2u, fused::get<1>(x));
  EXPECT_EQ('3', fused::get<2>(x));
}

void canComposeFusedTie() {
  auto a = 1;
  auto b = 2u;
  auto c = '3';
  auto x = fused::apply(fused::tie, a, b, c);
  static_assert(fused::tuple_size(fused::type_tag<decltype(x)>{}) == 3u, "");
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
#if 0
  {
    auto x = fused::compose(fused::make_tuple(fused::front, fused::make_tuple), 1, 2u, '3');
    EXPECT_EQ(1, x);
  }
#endif
}

void canComposeFusedBack() {
  auto a = fused::apply(fused::back, fused::make_tuple(1, 2u, '3'));
  EXPECT_EQ('3', a);
  auto const& b = fused::apply(fused::back, 1, 2u, a);
  EXPECT_EQ('3', b);
  EXPECT_EQ(&a, &b);
}

void canComposeFusedForEach() {
  {
    auto i = 0u;
    auto f = [&i](auto&&) { ++i; };
    fused::apply(fused::for_each, fused::make_tuple(f, 2u, '3'));
    fused::apply(fused::for_each, f, 2u, '3');
    EXPECT_EQ(4, i);
  }
  {
    auto i = 0u;
    auto f = [&i](auto&&) { ++i; };
    auto opt = fused::make_tuple(fused::for_each, fused::make_tuple);
    fused::compose(opt, f, 1, 2u, '3', 4.0);
    EXPECT_EQ(4, i);
  }
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
  {
    auto f = [](auto&& x) { return ++x; };
    auto x = fused::apply(fused::transform, f, 1, 2u, '3');
    static_assert(fused::tuple_size(fused::type_tag<decltype(x)>{}) == 3u, "");
    EXPECT_EQ(2, fused::get<0>(x));
    EXPECT_EQ(3u, fused::get<1>(x));
    EXPECT_EQ('4', fused::get<2>(x));
  }
  {
    auto f = [](auto&& x) { return std::make_unique<std::remove_reference_t<decltype(x)>>(x); };
    auto opt = fused::make_tuple(fused::transform, fused::make_tuple);
    auto x = fused::compose(opt, f, 1, 2u, '3', 4.0);
    static_assert(fused::tuple_size(fused::type_tag<decltype(x)>{}) == 4u, "");
    EXPECT_TRUE(fused::get<0>(x) != nullptr);
    EXPECT_EQ(1, *fused::get<0>(x));
  }
}

void canComposeFusedConcat() {
  auto x = fused::apply(fused::concat, fused::make_tuple(1, 2u, '3'), 4.0);
  static_assert(fused::tuple_size(fused::type_tag<decltype(x)>{}) == 4u, "");
  EXPECT_EQ(1, fused::get<0>(x));
  EXPECT_EQ(2u, fused::get<1>(x));
  EXPECT_EQ('3', fused::get<2>(x));
  EXPECT_EQ(4.0, fused::get<3>(x));

  auto y =
      fused::apply(fused::concat, fused::make_tuple(1, 2u, '3'), fused::make_tuple(4.0, 5, 6u));
  static_assert(fused::tuple_size(fused::type_tag<decltype(y)>{}) == 6u, "");
  EXPECT_EQ(1, fused::get<0>(y));
  EXPECT_EQ(2u, fused::get<1>(y));
  EXPECT_EQ('3', fused::get<2>(y));
  EXPECT_EQ(4.0, fused::get<3>(y));
  EXPECT_EQ(5, fused::get<4>(y));
  EXPECT_EQ(6u, fused::get<5>(y));
}

void canComposeFusedZip() {
  auto x =
      fused::apply(fused::concat, fused::make_tuple(1, 2u, '3'), fused::make_tuple(4.0, 5, 6u));
  static_assert(fused::tuple_size(fused::type_tag<decltype(x)>{}) == 6u, "");
  EXPECT_EQ(1, fused::get<0>(x));
  EXPECT_EQ(2u, fused::get<1>(x));
  EXPECT_EQ('3', fused::get<2>(x));
  EXPECT_EQ(4.0, fused::get<3>(x));
  EXPECT_EQ(5, fused::get<4>(x));
  EXPECT_EQ(6u, fused::get<5>(x));
}

void canComposeFusedTail() {
  auto x = fused::tail(fused::make_tuple(1, 2u, '3'));
  static_assert(fused::tuple_size(fused::type_tag<decltype(x)>{}) == 2u, "");
  EXPECT_EQ(2u, fused::get<0>(x));
  EXPECT_EQ('3', fused::get<1>(x));
}

void canComposeFusedFind() {
#if !defined(HAS_VARIABLE_TEMPLATES)
  constexpr auto& find_u = fused::find_v<unsigned>::value;
  constexpr auto& find_c = fused::find_v<char>::value;
#else
  constexpr auto& find_u = fused::find<unsigned>;
  constexpr auto& find_c = fused::find<char>;
#endif
  auto x = fused::apply(find_u, 1, 2u, '3', 4u);
  EXPECT_EQ(2u, x);
  auto y = fused::apply(find_c, 1, 2u, '3', 4u);
  EXPECT_EQ('3', y);
}

void canComposeFusedFindIf() {
#if !defined(HAS_VARIABLE_TEMPLATES)
  constexpr auto& find_iu = fused::find_if_v<std::is_unsigned>::value;
  constexpr auto& find_is = fused::find_if_v<std::is_signed>::value;
#else
  constexpr auto& find_iu = fused::find_if<std::is_unsigned>;
  constexpr auto& find_is = fused::find_if<std::is_signed>;
#endif
  auto x = fused::apply(find_iu, 1, 2u, '3', 4u);
  EXPECT_EQ(2u, x);
  auto y = fused::apply(find_is, 1, 2u, '3', 4u);
  EXPECT_EQ(1, y);
}

void canComposeFusedTake() {
#if !defined(HAS_VARIABLE_TEMPLATES)
  constexpr auto& take_2 = fused::take_v<2>::value;
  constexpr auto& take_3 = fused::take_v<3>::value;
#else
  constexpr auto& take_2 = fused::take<2>;
  constexpr auto& take_3 = fused::take<3>;
#endif
  auto x = fused::apply(take_2, fused::make_tuple(1, 2u, '3'));
  static_assert(fused::tuple_size(fused::type_tag<decltype(x)>{}) == 2u, "");
  EXPECT_EQ(1, fused::get<0>(x));
  EXPECT_EQ(2u, fused::get<1>(x));

  auto y = fused::apply(take_3, 4.0, '3', 2u, 1);
  static_assert(fused::tuple_size(fused::type_tag<decltype(y)>{}) == 3u, "");
  EXPECT_EQ(4.0, fused::get<0>(y));
  EXPECT_EQ('3', fused::get<1>(y));
  EXPECT_EQ(2u, fused::get<2>(y));
}

void canComposeFusedNth() {
#if !defined(HAS_VARIABLE_TEMPLATES)
  constexpr auto& fst = fused::nth_v<0>::value;
  constexpr auto& snd = fused::nth_v<1>::value;
  constexpr auto& trd = fused::nth_v<2>::value;
#else
  constexpr auto& fst = fused::nth<0>;
  constexpr auto& snd = fused::nth<1>;
  constexpr auto& trd = fused::nth<2>;
#endif
  auto x = fused::apply(fst, 1, 2u, '3');
  auto y = fused::apply(snd, 1, 2u, '3');
  auto z = fused::apply(trd, 1, 2u, '3');
  EXPECT_EQ(1, x);
  EXPECT_EQ(2u, y);
  EXPECT_EQ('3', z);
}

void canComposeFusedConstruct() {
#if !defined(HAS_VARIABLE_TEMPLATES)
  constexpr auto& ctor = fused::id<fused::tuple<int, unsigned, char>>::value;
#else
  constexpr auto& ctor = fused::id<fused::tuple<int, unsigned, char>>;
#endif
  auto x = fused::apply(ctor, 1, 2u, '3');
  static_assert(fused::tuple_size(fused::type_tag<decltype(x)>{}) == 3u, "");
  EXPECT_EQ(1, fused::get<0>(x));
  EXPECT_EQ(2u, fused::get<1>(x));
  EXPECT_EQ('3', fused::get<2>(x));
}

void canComposeIndexOf() {
#if !defined(HAS_VARIABLE_TEMPLATES)
  constexpr auto& idx_i32 = fused::index_of<int>::value;
  constexpr auto& idx_u32 = fused::index_of<unsigned>::value;
  constexpr auto& idx_u8 = fused::index_of<char>::value;
  constexpr auto& tl = fused::type_list<int, unsigned, char>::value;
#else
  constexpr auto& idx_i32 = fused::index_of<int>;
  constexpr auto& idx_u32 = fused::index_of<unsigned>;
  constexpr auto& idx_u8 = fused::index_of<char>;
  constexpr auto& tl = fused::type_list<int, unsigned, char>;
#endif
  {
    auto x = fused::apply(idx_i32, 1, 2u, '3');
    auto y = fused::apply(idx_u32, 1, 2u, '3');
    auto z = fused::apply(idx_u8, 1, 2u, '3');
    EXPECT_EQ(0, x);
    EXPECT_EQ(1, y);
    EXPECT_EQ(2, z);
  }
  {
    auto x = fused::apply(idx_i32, 1);
    EXPECT_EQ(0, x);
  }
  {
    auto x = fused::apply(idx_i32, tl);
    auto y = fused::apply(idx_u32, tl);
    auto z = fused::apply(idx_u8, tl);
    EXPECT_EQ(0, x);
    EXPECT_EQ(1, y);
    EXPECT_EQ(2, z);
  }
}

int main() {
  canUseFusedCompose();
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
  canComposeFusedFind();
  canComposeFusedFindIf();
  canComposeFusedTake();
  canComposeFusedNth();
  canComposeFusedConstruct();
  canComposeIndexOf();

  return 0;
}
