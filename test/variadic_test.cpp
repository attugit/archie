#include <cstdint>
#include <utility>

#include <archie/utils/variadic.h>

#include <gtest/gtest.h>
namespace au = archie::utils;

namespace {

struct Aqq {
  Aqq() = delete;
  Aqq(int) {}
};

struct variadic_test : ::testing::Test {};

TEST_F(variadic_test, canCreateTuple) {
  auto t0 = au::Tuple<char, int>();
  auto t1 = au::Tuple<char>('d');
  EXPECT_EQ(2u, t0.size());

  EXPECT_EQ(1u, t1.size());
  EXPECT_EQ('d', au::get<0>(t1));

  auto t3 = au::Tuple<int, float, char, unsigned>(1, 3.0f, 'c', 2u);
  EXPECT_EQ(4u, t3.size());
  EXPECT_EQ(1, au::get<0>(t3));
  EXPECT_EQ(3.0f, au::get<1>(t3));
  EXPECT_EQ('c', au::get<2>(t3));
  EXPECT_EQ(2u, au::get<3>(t3));
}

TEST_F(variadic_test, for_each) {
  int idx = 0;
  au::for_each([&idx](auto&&) { ++idx; }, 7, 'c', std::string{});
  EXPECT_EQ(3, idx);
}

TEST_F(variadic_test, type_index) {
  EXPECT_EQ(0u, (au::Variadic<char, int, float>::index_of<char>()));
  EXPECT_EQ(1u, (au::Variadic<char, int, float>::index_of<int>()));
  EXPECT_EQ(2u, (au::Variadic<char, int, float>::index_of<float>()));
}
}
