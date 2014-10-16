#include <archie/utils/typeset.h>
#include <archie/utils/get.h>

#include <gtest/gtest.h>
namespace au = archie::utils;
struct typeset_test : ::testing::Test {};

TEST_F(typeset_test, nothing) {
  au::TypeSet<unsigned> unary(1);
  EXPECT_EQ(1u, std::get<unsigned>(unary));

  au::TypeSet<unsigned, float> binary(2, 3.0);
  EXPECT_EQ(2u, std::get<unsigned>(binary));
  EXPECT_EQ(3.0, std::get<float>(binary));

  std::get<unsigned>(binary) = 4;
  std::get<float>(binary) = 5.0;

  EXPECT_EQ(4u, std::get<unsigned>(binary));
  EXPECT_EQ(5.0, std::get<float>(binary));

  au::TypeSet<unsigned, double> lhs(1, 2.0);
  au::TypeSet<unsigned, double> rhs(1, 3.0);
  au::TypeSet<unsigned, double> third(2, 1.0);
  au::TypeSet<unsigned, double> fourth(2, 1.0);
  EXPECT_TRUE(lhs < rhs);
  EXPECT_FALSE(lhs < lhs);
  EXPECT_FALSE(rhs < rhs);
  EXPECT_TRUE(lhs < third);
  EXPECT_TRUE(rhs < third);
  EXPECT_FALSE(third < lhs);
  EXPECT_TRUE(lhs == lhs);
  EXPECT_TRUE(third == fourth);
}
