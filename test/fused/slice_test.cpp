#include <archie/fused/slice.hpp>
#include <type_traits>
#include <catch.hpp>

namespace
{
  namespace fused = archie::fused;

  template <unsigned>
  struct utype {
  };
  using _0 = utype<0>;
  using _1 = utype<1>;

  using x = decltype(fused::slice<0, 1>(fused::tuple<_0, _1>{}));
  static_assert(std::is_same<x, fused::tuple<_0>>::value);
  using y = decltype(fused::slice<0, 2>(fused::tuple<_0, _1>{}));
  static_assert(std::is_same<y, fused::tuple<_0, _1>>::value);
  using z = decltype(fused::slice<1, 2>(fused::tuple<_0, _1>{}));
  static_assert(std::is_same<z, fused::tuple<_1>>::value);

  TEST_CASE("canTakeElementsFromTuple", "[fused::take]")
  {
    auto a = fused::make_tuple(1, 2.0, '3');
    auto b = fused::slice<0, 1>(a);
    auto c = fused::slice<0, 2>(a);
    auto d = fused::slice<0, 3>(a);

    REQUIRE(1 == fused::get<0>(b));
    REQUIRE(1 == fused::get<0>(c));
    REQUIRE(2.0 == fused::get<1>(c));
    REQUIRE(1 == fused::get<0>(d));
    REQUIRE(2.0 == fused::get<1>(d));
    REQUIRE('3' == fused::get<2>(d));
  }
}
