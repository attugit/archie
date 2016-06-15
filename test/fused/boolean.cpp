#include <archie/meta.hpp>
#include <gtest/gtest.h>

namespace
{
  namespace fused = archie::fused;

  static_assert(fused::True != fused::False);
  static_assert(fused::False != fused::True);
  static_assert(fused::True == fused::True);
  static_assert(fused::False == fused::False);

  TEST(boolean, canUseFusedBoolean)
  {
    auto const true_ = std::integral_constant<bool, true>{};
    auto const false_ = std::integral_constant<bool, false>{};

    EXPECT_TRUE(fused::True);
    EXPECT_FALSE(fused::False);
    EXPECT_NE(fused::True, fused::False);
    EXPECT_EQ(fused::True, true_);
    EXPECT_EQ(fused::False, false_);
    EXPECT_NE(fused::True, false_);
    EXPECT_NE(fused::False, true_);
    EXPECT_EQ(fused::True, fused::True);
    EXPECT_EQ(fused::False, fused::False);
  }
}
