#include <archie/enumerate.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <memory>

namespace
{
  TEST(enumerate, canEnumerateNonConstVector)
  {
    static std::size_t const value = 1u;
    std::vector<std::size_t> v(3, value);
    std::size_t i = 0;
    for (auto item : archie::enumerate(v)) {
      EXPECT_EQ(value, item.second);
      EXPECT_EQ(std::addressof(v[i]), std::addressof(item.second));
      EXPECT_EQ(i++, item.first);
      item = item.first;
      EXPECT_EQ(item.first, item.second);
    }
  }

  TEST(enumerate, canEnumerateConstVector)
  {
    static std::size_t const value = 1u;
    std::vector<std::size_t> const v(3, value);
    std::size_t i = 0;
    for (auto item : archie::enumerate(v)) {
      EXPECT_EQ(value, item.second);
      EXPECT_EQ(std::addressof(v[i]), std::addressof(item.second));
      EXPECT_EQ(i++, item.first);
    }
  }
}
