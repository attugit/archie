#include <archie/fused/back.hpp>
#include <memory>
#include <gtest/gtest.h>

namespace {
namespace meta = archie::meta;
namespace fused = archie::fused;

TEST(back, canUseBackWithRValue)
{
  auto a = fused::back(std::make_unique<unsigned>(1u), 2.0, '3');
  EXPECT_EQ('3', a);
}

TEST(back, canUseBackWithLValue)
{
  auto a = std::make_unique<unsigned>(1u);
  double b = 2.0;
  char c = '3';
  auto const& x = fused::back(a, b, c);
  EXPECT_EQ('3', x);
  EXPECT_EQ(&c, &x);
}
}
