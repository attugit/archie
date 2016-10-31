#include <archie/filter.hpp>
#include <gtest/gtest.h>
#include <vector>

namespace
{
  TEST(filter, canFilterConstVector)
  {
    auto const is_odd = [](auto const& x) { return (x % 2) != 0; };
    std::vector<std::size_t> const v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<std::size_t> const expected = {1, 3, 5, 7, 9};
    auto it = std::begin(expected);
    for (auto const& item : archie::filter(v, is_odd)) {
      EXPECT_EQ(*it++, item);
    }
  }
  TEST(filter, canFilterNonConstVector)
  {
    std::vector<std::size_t> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    {
      auto const is_odd = [](auto const& x) { return (x % 2) != 0; };
      std::vector<std::size_t> const expected = {1, 3, 5, 7, 9};
      auto it = std::begin(expected);
      for (auto& item : archie::filter(v, is_odd)) {
        EXPECT_EQ(*it++, item);
        item = 10;
      }
    }
    std::vector<std::size_t> const expected = {10, 2, 10, 4, 10, 6, 10, 8, 10};
    EXPECT_EQ(expected, v);
  }
}
