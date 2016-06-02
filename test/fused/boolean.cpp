#include <archie/boolean.hpp>
#include <catch.hpp>

namespace
{
  namespace fused = archie::fused;

  static_assert(fused::True != fused::False);
  static_assert(fused::False != fused::True);
  static_assert(fused::True == fused::True);
  static_assert(fused::False == fused::False);

  TEST_CASE("canUseFusedBoolean", "[fused::boolean]")
  {
    auto const true_ = std::integral_constant<bool, true>{};
    auto const false_ = std::integral_constant<bool, false>{};

    REQUIRE(fused::True);
    REQUIRE_FALSE(fused::False);
    REQUIRE(fused::True != fused::False);
    REQUIRE(fused::True == true_);
    REQUIRE(fused::False == false_);
    REQUIRE(fused::True != false_);
    REQUIRE(fused::False != true_);
    REQUIRE(fused::True == fused::True);
    REQUIRE(fused::False == fused::False);
  }
}
