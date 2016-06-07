#include <archie/number.hpp>
#include <gtest/gtest.h>

namespace {
namespace fused = archie::fused;
TEST(number, canUseFusedNumber)
{
  EXPECT_EQ(0u, fused::number<0>);
  EXPECT_EQ(1u, fused::number<1>);
  EXPECT_EQ(2u, fused::number<2>);
}
}
