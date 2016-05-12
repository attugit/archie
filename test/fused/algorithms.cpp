#include <archie/fused/tuple.hpp>
#include <archie/fused/algorithm.hpp>
#include <memory>
#include <catch.hpp>

namespace {
namespace fused = archie::fused;

TEST_CASE("canUseFusedCompose", "[fused::algo]") {
  {
    auto x = fused::compose(fused::make_tuple, 1, 2u, '3');
    static_assert(fused::tuple_size(fused::id<decltype(x)>) == 3u, "");
    REQUIRE(1 == fused::get<0>(x));
    REQUIRE(2u == fused::get<1>(x));
    REQUIRE('3' == fused::get<2>(x));
  }
#if 0
  {
    auto a = 1;
    auto b = 2u;
    auto c = '3';

    auto const& x = fused::compose(fused::make_tuple(fused::front, fused::tie), a, b, c);
    REQUIRE(&a == &x);
  }
  {
    auto a = 1;
    auto b = 2u;
    auto c = '3';

    auto opt = fused::make_tuple(fused::front, fused::tie);
    auto const& x = fused::compose(opt, a, b, c);
    REQUIRE(&a == &x);
  }
#endif
}

TEST_CASE("canComposeFusedMakeTuple", "[fused::algo]") {
  auto x = fused::apply(fused::make_tuple, 1, 2u, '3');
  static_assert(fused::tuple_size(fused::id<decltype(x)>) == 3u, "");
  REQUIRE(1 == fused::get<0>(x));
  REQUIRE(2u == fused::get<1>(x));
  REQUIRE('3' == fused::get<2>(x));
}

TEST_CASE("canComposeFusedTie", "[fused::algo]") {
  auto a = 1;
  auto b = 2u;
  auto c = '3';
  auto x = fused::apply(fused::tie, a, b, c);
  static_assert(fused::tuple_size(fused::id<decltype(x)>) == 3u, "");
  REQUIRE(&a == &fused::get<0>(x));
  REQUIRE(&b == &fused::get<1>(x));
  REQUIRE(&c == &fused::get<2>(x));
}

TEST_CASE("canComposeFusedFront", "[fused::algo]") {
  {
    auto a = fused::apply(fused::front, fused::make_tuple(1, 2u, '3'));
    REQUIRE(1 == a);
    auto const& b = fused::apply(fused::front, a, 2u, '3');
    REQUIRE(1 == b);
    REQUIRE(&a == &b);
  }
#if 0
  {
    auto x = fused::compose(fused::make_tuple(fused::front, fused::make_tuple), 1, 2u, '3');
    REQUIRE(1 == x);
  }
#endif
}

TEST_CASE("canComposeFusedBack", "[fused::algo]") {
  auto a = fused::apply(fused::back, fused::make_tuple(1, 2u, '3'));
  REQUIRE('3' == a);
  auto const& b = fused::apply(fused::back, 1, 2u, a);
  REQUIRE('3' == b);
  REQUIRE(&a == &b);
}

TEST_CASE("canComposeFusedForEach", "[fused::algo]") {
  {
    auto i = 0u;
    auto f = [&i](auto&&) { ++i; };
    fused::apply(fused::for_each, fused::make_tuple(f, 2u, '3'));
    fused::apply(fused::for_each, f, 2u, '3');
    REQUIRE(4 == i);
  }
  {
    auto i = 0u;
    auto f = [&i](auto&&) { ++i; };
    auto opt = fused::make_tuple(fused::for_each, fused::make_tuple);
    fused::compose(opt, f, 1, 2u, '3', 4.0);
    REQUIRE(4 == i);
  }
}

TEST_CASE("canComposeFusedForEachOrder", "[fused::algo]") {
  auto i = 0u;
  auto f = [&i](auto&& x) {
    i *= 10;
    i += x;
  };
  fused::for_each_forward(f, 1u, 2u, 3u);
  REQUIRE(123u == i);
  i = 0u;
  fused::for_each_backward(f, 1u, 2u, 3u);
  REQUIRE(321u == i);
}

TEST_CASE("canComposeFusedTransform", "[fused::algo]") {
  {
    auto f = [](auto&& x) { return ++x; };
    auto x = fused::apply(fused::transform, f, 1, 2u, '3');
    static_assert(fused::tuple_size(fused::id<decltype(x)>) == 3u, "");
    REQUIRE(2 == fused::get<0>(x));
    REQUIRE(3u == fused::get<1>(x));
    REQUIRE('4' == fused::get<2>(x));
  }
  {
    auto f = [](auto&& x) { return std::make_unique<std::remove_reference_t<decltype(x)>>(x); };
    auto opt = fused::make_tuple(fused::transform, fused::make_tuple);
    auto x = fused::compose(opt, f, 1, 2u, '3', 4.0);
    static_assert(fused::tuple_size(fused::id<decltype(x)>) == 4u, "");
    REQUIRE(fused::get<0>(x) != nullptr);
    REQUIRE(1 == *fused::get<0>(x));
  }
}

TEST_CASE("canComposeFusedConcat", "[fused::algo]") {
  auto x = fused::apply(fused::concat, fused::make_tuple(1, 2u, '3'), 4.0);
  static_assert(fused::tuple_size(fused::id<decltype(x)>) == 4u, "");
  REQUIRE(1 == fused::get<0>(x));
  REQUIRE(2u == fused::get<1>(x));
  REQUIRE('3' == fused::get<2>(x));
  REQUIRE(4.0 == fused::get<3>(x));

  auto y =
      fused::apply(fused::concat, fused::make_tuple(1, 2u, '3'), fused::make_tuple(4.0, 5, 6u));
  static_assert(fused::tuple_size(fused::id<decltype(y)>) == 6u, "");
  REQUIRE(1 == fused::get<0>(y));
  REQUIRE(2u == fused::get<1>(y));
  REQUIRE('3' == fused::get<2>(y));
  REQUIRE(4.0 == fused::get<3>(y));
  REQUIRE(5 == fused::get<4>(y));
  REQUIRE(6u == fused::get<5>(y));
}

TEST_CASE("canComposeFusedZip", "[fused::algo]") {
  auto x =
      fused::apply(fused::concat, fused::make_tuple(1, 2u, '3'), fused::make_tuple(4.0, 5, 6u));
  static_assert(fused::tuple_size(fused::id<decltype(x)>) == 6u, "");
  REQUIRE(1 == fused::get<0>(x));
  REQUIRE(2u == fused::get<1>(x));
  REQUIRE('3' == fused::get<2>(x));
  REQUIRE(4.0 == fused::get<3>(x));
  REQUIRE(5 == fused::get<4>(x));
  REQUIRE(6u == fused::get<5>(x));
}

TEST_CASE("canComposeFusedTail", "[fused::algo]") {
  auto x = fused::tail(fused::make_tuple(1, 2u, '3'));
  static_assert(fused::tuple_size(fused::id<decltype(x)>) == 2u, "");
  REQUIRE(2u == fused::get<0>(x));
  REQUIRE('3' == fused::get<1>(x));
}

TEST_CASE("canComposeFusedFind", "[fused::algo]") {
  auto x = fused::apply(fused::find<unsigned>, 1, 2u, '3', 4u);
  REQUIRE(2u == x);
  auto y = fused::apply(fused::find<char>, 1, 2u, '3', 4u);
  REQUIRE('3' == y);
}

TEST_CASE("canComposeFusedFindIf", "[fused::algo]") {
  auto x = fused::apply(fused::find_if<std::is_unsigned>, 1, 2u, '3', 4u);
  REQUIRE(2u == x);
  auto y = fused::apply(fused::find_if<std::is_signed>, 1, 2u, '3', 4u);
  REQUIRE(1 == y);
}

TEST_CASE("canComposeFusedTake", "[fused::algo]") {
  auto x = fused::apply(fused::take<2>, fused::make_tuple(1, 2u, '3'));
  static_assert(fused::tuple_size(fused::id<decltype(x)>) == 2u, "");
  REQUIRE(1 == fused::get<0>(x));
  REQUIRE(2u == fused::get<1>(x));

  auto y = fused::apply(fused::take<3>, 4.0, '3', 2u, 1);
  static_assert(fused::tuple_size(fused::id<decltype(y)>) == 3u, "");
  REQUIRE(4.0 == fused::get<0>(y));
  REQUIRE('3' == fused::get<1>(y));
  REQUIRE(2u == fused::get<2>(y));
}

TEST_CASE("canComposeFusedNth", "[fused::algo]") {
  auto x = fused::apply(fused::nth<0>, 1, 2u, '3');
  auto y = fused::apply(fused::nth<1>, 1, 2u, '3');
  auto z = fused::apply(fused::nth<2>, 1, 2u, '3');
  REQUIRE(1 == x);
  REQUIRE(2u == y);
  REQUIRE('3' == z);
}

TEST_CASE("canComposeFusedConstruct", "[fused::algo]") {
  using tuple_type = fused::tuple<int, unsigned, char>;
  constexpr auto& ctor = fused::id<tuple_type>;
  auto x = fused::apply(ctor, 1, 2u, '3');
  static_assert(fused::tuple_size(fused::id<decltype(x)>) == 3u, "");
  REQUIRE(1 == fused::get<0>(x));
  REQUIRE(2u == fused::get<1>(x));
  REQUIRE('3' == fused::get<2>(x));
}

TEST_CASE("canComposeIndexOf", "[fused::algo]") {
  {
    auto x = fused::apply(fused::index_of<int>, 1, 2u, '3');
    auto y = fused::apply(fused::index_of<unsigned>, 1, 2u, '3');
    auto z = fused::apply(fused::index_of<char>, 1, 2u, '3');
    REQUIRE(0 == x);
    REQUIRE(1 == y);
    REQUIRE(2 == z);
  }
  {
    auto x = fused::apply(fused::index_of<int>, 1);
    REQUIRE(0 == x);
  }
  {
    auto x = fused::apply(fused::index_of<int>, fused::type_list<int, unsigned, char>);
    auto y = fused::apply(fused::index_of<unsigned>, fused::type_list<int, unsigned, char>);
    auto z = fused::apply(fused::index_of<char>, fused::type_list<int, unsigned, char>);
    REQUIRE(0 == x);
    REQUIRE(1 == y);
    REQUIRE(2 == z);
  }
}
}
