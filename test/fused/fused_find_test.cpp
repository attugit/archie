#include <archie/fused/find.h>
#include <type_traits>
#include <catch.hpp>

namespace {
namespace fused = archie::fused;

TEST_CASE("canUseFusedFind", "[fused::find]") {
  unsigned a = 0u;
  int b = 1;
  char c = '2';
  double d = 3.0;
  unsigned e = 4u;

  auto x = fused::find<unsigned&>(a, b, c, d, e);
  auto y = fused::find<double&>(a, b, c, d, e);
  REQUIRE(a == x);
  REQUIRE(d == y);

  REQUIRE(1 == fused::find<int>(1, 2u, 3.0, '4'));
  REQUIRE(2u == fused::find<unsigned>(1, 2u, 3.0, '4'));
  REQUIRE(3.0 == fused::find<double>(1, 2u, 3.0, '4'));
  REQUIRE('4' == fused::find<char>(1, 2u, 3.0, '4'));
}

template <typename Tp>
using is_u = std::is_unsigned<std::decay_t<Tp>>;

template <typename Tp>
using is_s = std::is_signed<std::decay_t<Tp>>;

TEST_CASE("canUseFusedFindIf", "[fused::find]") {
  unsigned a = 0u;
  int b = 1;
  char c = '2';
  double d = 3.0;
  unsigned e = 4u;

  auto x = fused::find_if<is_u>(a, b, c, d, e);
  auto y = fused::find_if<is_s>(a, b, c, d, e);
  REQUIRE(a == x);
  REQUIRE(b == y);
}

template <typename F, typename... Ts>
decltype(auto) foo(F&& f, Ts&&... ts) {
  return std::forward<F>(f)(std::forward<Ts>(ts)...);
}

TEST_CASE("canUseVariableTemplateFind", "[fused::find]") {
  {
    auto x = foo(fused::find<int>, 1, 2u, '3', 4.0, 5);
    static_assert(std::is_same<decltype(x), int>::value, "");
    REQUIRE(1 == x);
  }
  {
    auto x = foo(fused::find_if<is_u>, 1, 2u, '3', 4.0, 5);
    static_assert(std::is_same<decltype(x), unsigned>::value, "");
    REQUIRE(2u == x);
  }
}
}
