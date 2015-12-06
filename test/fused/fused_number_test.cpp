#include <archie/fused/number.hpp>
#include <catch.hpp>

namespace {
namespace fused = archie::fused;
TEST_CASE("canUseFusedNumber", "[fused::number]") {
  REQUIRE(0u == fused::number<0>);
  REQUIRE(1u == fused::number<1>);
  REQUIRE(2u == fused::number<2>);
}
}
