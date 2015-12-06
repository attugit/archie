#include <archie/fused/back.hpp>
#include <memory>
#include <catch.hpp>

namespace {
namespace meta = archie::meta;
namespace fused = archie::fused;

TEST_CASE("canUseBackWithRValue") {
  auto a = fused::back(std::make_unique<unsigned>(1u), 2.0, '3');
  REQUIRE('3' == a);
}

TEST_CASE("canUseBackWithLValue") {
  auto a = std::make_unique<unsigned>(1u);
  double b = 2.0;
  char c = '3';
  auto const& x = fused::back(a, b, c);
  REQUIRE('3' == x);
  REQUIRE(&c == &x);
}
}
