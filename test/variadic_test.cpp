#include <cstdint>
#include <utility>
#include <list>
#include <memory>

#include <archie/utils/variadic.h>
#include <archie/utils/tuple.h>

#include <gtest/gtest.h>
namespace au = archie::utils;

namespace {

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

TEST_F(variadic_test, reference_via_get) {
  auto tuple = au::Tuple<char, int, float>('c', 7, 3.0f);
  ASSERT_EQ('c', au::get<0>(tuple));
  ASSERT_EQ(7, au::get<1>(tuple));
  ASSERT_EQ(3.0f, au::get<2>(tuple));
}

struct func {
  int cnt = 0;
  template <typename Tp>
  void operator()(Tp&&) noexcept {
    ++cnt;
  }
};

TEST_F(variadic_test, for_each) {
  int idx = 0;
  au::for_each([&idx](auto&&) { ++idx; }, 7, 'c', std::string{});
  EXPECT_EQ(3, idx);

  func f;
  ASSERT_EQ(0, f.cnt);
  au::for_each(f);
  EXPECT_EQ(0, f.cnt);
  auto& ret = au::for_each(f, '7', 'c', std::string{});
  EXPECT_EQ(3, f.cnt);
  EXPECT_EQ(3, ret.cnt);
  EXPECT_EQ(&f, &ret);
}

TEST_F(variadic_test, type_index) {
  EXPECT_EQ(0u, (au::Variadic<char, int, float>::index_of<char>()));
  EXPECT_EQ(1u, (au::Variadic<char, int, float>::index_of<int>()));
  EXPECT_EQ(2u, (au::Variadic<char, int, float>::index_of<float>()));

  auto tuple = au::Tuple<char, int, float>('c', 7, 3.0f);
  EXPECT_EQ('c', au::get<char>(tuple));
  EXPECT_EQ(7, au::get<int>(tuple));
  EXPECT_EQ(3.0f, au::get<float>(tuple));
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

TEST_F(variadic_test, tuple_for_each) {
  auto tuple = au::Tuple<int, int, int>(1, 2, 3);
  unsigned sum = 0;
  auto func = [&sum](auto x) { sum += x; };
  tuple.for_each(func);
  EXPECT_EQ(6u, sum);
}

TEST_F(variadic_test, back) {
  EXPECT_EQ(1, au::back(3, 2, 1));
  EXPECT_EQ(2u, au::back(4.0f, 3ul, 2u));
  EXPECT_EQ(std::string("a"), au::back(1, std::cout, std::string("a")));
}
}
