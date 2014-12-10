#include <gtest/gtest.h>
#include <archie/utils/fused/back.h>

namespace {
namespace fused = archie::utils::fused;
struct back_test : ::testing::Test {};

TEST_F(back_test, canUseBackWithRValue) {
  auto a = fused::back(1u, 2.0, '3');
  EXPECT_EQ('3', a);
}

TEST_F(back_test, canUseBackWithLValue) {
  unsigned a = 1u;
  double b = 2.0;
  char c = '3';
  auto const& x = fused::back(a, b, c);
  EXPECT_EQ('3', x);
  EXPECT_EQ(&c, &x);
}
}
