#include <archie/fused/as_tuple.hpp>
#include <type_traits>
#include <catch.hpp>

namespace {
namespace meta = archie::meta;
namespace fused = archie::fused;

template <unsigned>
struct utype {
};
using _0 = utype<0>;
using _1 = utype<1>;

static_assert(std::is_same<fused::as_tuple<_0>::type, fused::tuple<_0>>::value, "");
static_assert(std::is_same<fused::as_tuple<_0, _1>::type, fused::tuple<_0, _1>>::value, "");
static_assert(std::is_same<fused::as_tuple<_0, _1, _0>::type, fused::tuple<_0, _1, _0>>::value, "");
static_assert(std::is_same<fused::as_tuple<meta::type_list<_0>>::type, fused::tuple<_0>>::value,
              "");
static_assert(
    std::is_same<fused::as_tuple<meta::type_list<_0, _1>>::type, fused::tuple<_0, _1>>::value,
    "");
static_assert(std::is_same<fused::as_tuple<meta::type_list<_0, _1, _0>>::type,
                           fused::tuple<_0, _1, _0>>::value,
              "");

TEST_CASE("canUseAsTuple", "[fused::as_tuple]")
{
  auto x = fused::as_tuple<int, double, char>::make(1, 2.0, '3');

  REQUIRE(1 == fused::get<0>(x));
  REQUIRE(2.0 == fused::get<1>(x));
  REQUIRE('3' == fused::get<2>(x));
}

TEST_CASE("canUseAsTupleWithTypeList", "[fused::as_tuple]")
{
  auto x = fused::as_tuple<meta::type_list<int, double, char>>::make(1, 2.0, '3');

  REQUIRE(1 == fused::get<0>(x));
  REQUIRE(2.0 == fused::get<1>(x));
  REQUIRE('3' == fused::get<2>(x));
}
}
