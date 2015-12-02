#include <archie/utils/fused/number.h>
#include <catch.hpp>

namespace {
namespace fused = archie::utils::fused;
TEST_CASE("canUseFusedNumber", "[fused::number]") {
  REQUIRE(0u == fused::number<0>);
  REQUIRE(1u == fused::number<1>);
  REQUIRE(2u == fused::number<2>);
}
}
