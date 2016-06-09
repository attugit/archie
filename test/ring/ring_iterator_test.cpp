#include <archie/container/ring_iterator.hpp>
#include <gtest/gtest.h>
namespace {
using namespace archie;
TEST(ring_iterator, ring)
{
  using sut = ring_iterator<std::vector<int>::iterator>;
  {
    std::vector<int> vec = {0, 1, 2, 3, 4, 5, 6};
    sut it0;
    sut it1(std::begin(vec), std::end(vec));
    sut it2(std::begin(vec), std::end(vec), 2);
    sut it3(std::begin(vec), std::end(vec), 2 + vec.size());
    EXPECT_NE(it0, it1);
    EXPECT_NE(it0, it1);
    EXPECT_NE(it1, it2);
    EXPECT_NE(it1, it2);
    EXPECT_NE(it2, it3);
    EXPECT_NE(it2, it3);
    EXPECT_EQ((&(*it2)), (&(*it3)));
  }
  {
    using buff_t = std::vector<int>;
    buff_t buff = {0, 1, 2, 3, 4, 5, 6};
    ring_iterator<buff_t::const_iterator> it(buff.begin() + 1, buff.begin() + 4);
    EXPECT_EQ(*it++, 1);
    EXPECT_EQ(*it++, 2);
    EXPECT_EQ(*it++, 3);
    EXPECT_EQ(*it++, 1);
  }
}
}
