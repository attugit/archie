#include <archie/utils/typeset.h>

#include <gtest/gtest.h>
namespace au = archie::utils;
struct typeset_test : ::testing::Test {};

TEST_F(typeset_test, nothing) {
  au::TypeSet<unsigned> unary(1);
  EXPECT_EQ(1, unary.get<unsigned>());
  EXPECT_EQ(1, unary.size());

  au::TypeSet<unsigned, float> binary(2, 3.0);
  EXPECT_EQ(2, binary.get<unsigned>());
  EXPECT_EQ(3.0, binary.get<float>());

  binary.get<unsigned>() = 4;
  binary.get<float>() = 5.0;

  EXPECT_EQ(4, binary.get<unsigned>());
  EXPECT_EQ(5.0, binary.get<float>());
  EXPECT_EQ(2, binary.size());

  unsigned u = 7;
  float f = 8.0;

  au::TypeSet<float, unsigned> other(f, u);
  EXPECT_EQ(7, au::get<unsigned>(other));
  EXPECT_EQ(8.0, au::get<float>(other));

  auto triplet = au::make_type_set(1, 2.0, std::string("name"));
  EXPECT_EQ(1, au::get<int>(triplet));
  EXPECT_EQ(2.0, au::get<double>(triplet));
  EXPECT_EQ("name", au::get<std::string>(triplet));
  static_assert(au::type_set_size(triplet) == 3, "");
}
