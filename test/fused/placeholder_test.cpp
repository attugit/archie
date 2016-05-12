#include <archie/fused/placeholder.hpp>
#include <catch.hpp>

namespace {
namespace fused = archie::fused;
using namespace fused::placeholders;
TEST_CASE("canAccessArgumentWithPlaceholder")
{
  auto a = 1u;
  auto b = 2.0;
  auto c = '3';

  auto const& x = _0(a, b, c);
  auto const& y = _1(a, b, c);
  auto const& z = _2(a, b, c);

  REQUIRE(&a == &x);
  REQUIRE(&b == &y);
  REQUIRE(&c == &z);
}

TEST_CASE("canBindPlaceHolders")
{
  auto inc = [](auto&& x, auto&& y, auto&& z) {
    ++x;
    ++++y;
    ++++++z;
  };

  auto func = fused::reorder(inc, _1, _2, _0);

  auto a = 1u;
  auto b = 2.0;
  auto c = '3';

  func(a, b, c);

  REQUIRE(4u == a);
  REQUIRE(3.0 == b);
  REQUIRE('5' == c);
}

TEST_CASE("canGetExpiringValue")
{
  auto x = 0.0;
  {
    auto a = 1;
    auto b = 2u;
    auto c = 'c';
    auto d = 4.0;
    auto&& y = _0(std::move(a), b, c, d);
    x = _2(std::move(b), std::move(c), std::move(d));
    static_assert(std::is_same<decltype(y), int&&>::value, "");
  }
  REQUIRE(4.0 == x);
}
}
