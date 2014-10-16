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
}

TEST_F(variadic_test, for_each) {
  int idx = 0;
  au::for_each([&idx](auto&&) { ++idx; }, 7, 'c', std::string{});
  EXPECT_EQ(3, idx);
}
}
