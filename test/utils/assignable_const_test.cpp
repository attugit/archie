#include <archie/assignable_const.hpp>
#include <gtest/gtest.h>

namespace
{
  using namespace archie;

  struct block_t {
    block_t(unsigned n) : number(n) {}
    assignable_const<unsigned> number;
  };

  TEST(assignable_const, isConst)
  {
    block_t b1(3);
    block_t b2(4);

    EXPECT_EQ(b1.number, 3u);
    b1 = b2;
    EXPECT_EQ(b1.number, 4u);

    static_assert(std::is_copy_constructible<block_t>::value);
    static_assert(std::is_copy_assignable<block_t>::value);
  }
}
