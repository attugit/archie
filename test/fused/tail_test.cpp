#include <archie/fused/tail.hpp>
#include <archie/fused/tuple.hpp>
#include <catch.hpp>

namespace {
namespace fused = archie::fused;

TEST_CASE("canGetLValueFusedTupleTail", "[fused::tail]") {
  auto t0 = fused::make_tuple(1, 2u, '3');
  auto x = fused::tail(t0);
  static_assert(fused::tuple_size(fused::id<decltype(x)>) == 2u, "");
  REQUIRE(2u == fused::get<0>(x));
  REQUIRE('3' == fused::get<1>(x));
}

TEST_CASE("canGetRValueFusedTupleTail", "[fused::tail]") {
  auto x = fused::tail(fused::make_tuple(1, 2u, '3'));
  static_assert(fused::tuple_size(fused::id<decltype(x)>) == 2u, "");
  REQUIRE(2u == fused::get<0>(x));
  REQUIRE('3' == fused::get<1>(x));
}

TEST_CASE("canGetConstValueFusedTupleTail", "[fused::tail]") {
  auto const t0 = fused::make_tuple(1, 2u, '3');
  auto x = fused::tail(t0);
  static_assert(fused::tuple_size(fused::id<decltype(x)>) == 2u, "");
  REQUIRE(2u == fused::get<0>(x));
  REQUIRE('3' == fused::get<1>(x));
}

TEST_CASE("canGetFusedTupleOfReferencesTail", "[fused::tail]") {
  auto a = 1;
  auto b = 2u;
  auto c = '3';
  auto t0 = fused::tie(a, b, c);
  auto x = fused::tail(t0);
  static_assert(fused::tuple_size(fused::id<decltype(x)>) == 2u, "");
  REQUIRE(&b == &fused::get<0>(x));
  REQUIRE(&c == &fused::get<1>(x));
}
}
