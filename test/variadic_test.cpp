#include <utility>

#include <archie/utils/variadic.h>

#include <gtest/gtest.h>
namespace au = archie::utils;

namespace {

struct variadic_test : ::testing::Test {};

TEST_F(variadic_test, type_index) {
  EXPECT_EQ(0u, (au::Variadic<char, int, float>::index_of<char>()));
  EXPECT_EQ(1u, (au::Variadic<char, int, float>::index_of<int>()));
  EXPECT_EQ(2u, (au::Variadic<char, int, float>::index_of<float>()));
}

template <typename... Ts>
struct SizeOf {
  enum { value = sizeof...(Ts) };
};

TEST_F(variadic_test, meta_apply) {
  EXPECT_EQ(0u, (au::Variadic<>::Apply<SizeOf>::value));
  EXPECT_EQ(1u, (au::Variadic<char>::Apply<SizeOf>::value));
  EXPECT_EQ(2u, (au::Variadic<char, int>::Apply<SizeOf>::value));
  EXPECT_EQ(3u, (au::Variadic<char, int, float>::Apply<SizeOf>::value));
}
}
