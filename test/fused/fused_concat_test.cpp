#include <archie/fused/concat.h>
#include <catch.hpp>

namespace {
namespace fused = archie::fused;

TEST_CASE("canConcatTuple", "[fused::concat]") {
  {
    auto a = fused::make_tuple(1, 2u, '3');
    auto x = fused::concat(a, 4.0);

    REQUIRE(4u == fused::tuple_size(x));
    REQUIRE(1 == fused::get<0>(x));
    REQUIRE(2u == fused::get<1>(x));
    REQUIRE('3' == fused::get<2>(x));
    REQUIRE(4.0 == fused::get<3>(x));
  }
  {
    auto a = fused::make_tuple(1, 2u, '3');
    auto x = fused::concat(a, 4.0, 5, 6u);

    REQUIRE(6u == fused::tuple_size(x));
    REQUIRE(1 == fused::get<0>(x));
    REQUIRE(2u == fused::get<1>(x));
    REQUIRE('3' == fused::get<2>(x));
    REQUIRE(4.0 == fused::get<3>(x));
    REQUIRE(5 == fused::get<4>(x));
    REQUIRE(6u == fused::get<5>(x));
  }
}

TEST_CASE("canConcatTwoTuples", "[fused::concat]") {
  auto a = fused::make_tuple(1, 2u, '3');
  auto b = fused::make_tuple(4.0, 5, 6u);
  auto x = fused::concat(a, b);

  REQUIRE(6u == fused::tuple_size(x));
  REQUIRE(1 == fused::get<0>(x));
  REQUIRE(2u == fused::get<1>(x));
  REQUIRE('3' == fused::get<2>(x));
  REQUIRE(4.0 == fused::get<3>(x));
  REQUIRE(5 == fused::get<4>(x));
  REQUIRE(6u == fused::get<5>(x));
}
}
