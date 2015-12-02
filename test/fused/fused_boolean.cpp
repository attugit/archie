#include <archie/utils/fused/boolean.h>
#include <catch.hpp>

namespace {
namespace fused = archie::utils::fused;
TEST_CASE("canUseFusedBoolean", "[fused::boolean]") {
  REQUIRE(fused::True);
  REQUIRE_FALSE(fused::False);
}
}
