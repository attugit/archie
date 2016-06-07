#include <archie/fused/for_each.hpp>
#include <string>
#include <gtest/gtest.h>

namespace {
namespace fused = archie::fused;

struct func {
  int cnt = 0;
  template <typename Tp>
  void operator()(Tp&&) noexcept
  {
    ++cnt;
  }
};

TEST(for_each, canCallForEachWithFunctionObject)
{
  func f;
  EXPECT_EQ(0, f.cnt);
  fused::for_each(f);
  EXPECT_EQ(0, f.cnt);
  auto& ret = fused::for_each(f, '7', 'c', std::string{});
  EXPECT_EQ(3, f.cnt);
  EXPECT_EQ(3, ret.cnt);
  EXPECT_EQ(&f, &ret);
}

TEST(for_each, canCallForEachWithLambda)
{
  int idx = 0;
  auto f = [&idx](auto&&) { ++idx; };
  EXPECT_EQ(0, idx);
  fused::for_each(f, 7, 'c', 2.0);
  EXPECT_EQ(3, idx);
}
}
