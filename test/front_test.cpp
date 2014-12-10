#include <gtest/gtest.h>
#include <archie/utils/fused/front.h>

namespace {
namespace fused = archie::utils::fused;
struct front_test : ::testing::Test {};

TEST_F(front_test, canUseFrontWithRValue) {
  auto a = fused::front(1u, 2.0, '3');
  EXPECT_EQ(1u, a);
}

TEST_F(front_test, canUseBackWithLValue) {
  unsigned a = 1u;
  double b = 2.0;
  char c = '3';
  auto const& x = fused::front(a, b, c);
  EXPECT_EQ(1u, x);
  EXPECT_EQ(&a, &x);
}
}
