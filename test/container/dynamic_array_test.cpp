#include <archie/container/stack_buffer.hpp>
#include <archie/container/heap_buffer.hpp>
#include <resource.hpp>
#include <gtest/gtest.h>
namespace {
using namespace archie;
TEST(dynamic_array, stack_buffer)
{
  enum : std::size_t
  {
    stack_size = 7
  };
  using sut = stack_buffer<test::resource, stack_size>;
  using value_t = typename sut::value_type;
  {
    sut buff;
    EXPECT_EQ(buff.capacity(), stack_size);
    EXPECT_TRUE(buff.empty());
  }
  {
    sut buff;
    buff.emplace_back(1);
    EXPECT_TRUE(!buff.empty());
    EXPECT_EQ(buff.size(), 1u);
    EXPECT_EQ(buff[0], 1);
  }
  {
    sut buff = {value_t(1), value_t(2), value_t(3), value_t(4), value_t(5)};
    EXPECT_EQ(buff.size(), 5u);
    EXPECT_EQ(buff[0], 1);
    EXPECT_EQ(buff[1], 2);
    EXPECT_EQ(buff[2], 3);
    EXPECT_EQ(buff[3], 4);
    EXPECT_EQ(buff[4], 5);
  }
  {
    sut const orig = {value_t(1), value_t(2), value_t(3)};
    sut cpy{orig};
    EXPECT_EQ(cpy, orig);
  }
  {
    sut const ref = {value_t(1), value_t(2), value_t(3)};
    sut orig{ref};
    sut cpy{std::move(orig)};
    EXPECT_EQ(cpy, ref);
  }
  {
    sut const orig = {value_t(1), value_t(2), value_t(3)};
    sut cpy0;
    sut cpy1 = {value_t(4)};
    sut cpy2 = {value_t(5), value_t(6), value_t(7)};
    sut cpy3 = {value_t(8), value_t(9), value_t(10), value_t(11)};

    cpy0 = orig;
    EXPECT_EQ(cpy0, orig);
    cpy1 = orig;
    EXPECT_EQ(cpy1, orig);
    cpy2 = orig;
    EXPECT_EQ(cpy2, orig);
    cpy3 = orig;
    EXPECT_EQ(cpy3, orig);
    cpy3 = cpy3;
    EXPECT_EQ(cpy3, orig);
  }
  {
    sut lhs = {value_t(1), value_t(2), value_t(3)};
    sut rhs;
    rhs.emplace_back(1);
    rhs.emplace_back(2);
    rhs.emplace_back(3);
    EXPECT_EQ(lhs, rhs);
    rhs.emplace_back(4);
    EXPECT_NE(lhs, rhs);
  }
}
TEST(dynamic_array, heap_buffer)
{
  using sut = heap_buffer<test::resource>;
  using value_t = typename sut::value_type;
  {
    sut buff;
    EXPECT_EQ(buff.capacity(), 0u);
    EXPECT_TRUE(buff.empty());
  }
  {
    sut buff(7);
    EXPECT_EQ(buff.capacity(), 7u);
    EXPECT_TRUE(buff.empty());
  }
  {
    sut buff(7);
    buff.emplace_back(1);
    EXPECT_TRUE(!buff.empty());
    EXPECT_EQ(buff.size(), 1u);
    EXPECT_EQ(buff[0], 1);
  }
  {
    sut buff = {value_t(1), value_t(2), value_t(3)};
    EXPECT_EQ(buff.size(), 3u);
    EXPECT_EQ(buff[0], 1);
    EXPECT_EQ(buff[1], 2);
    EXPECT_EQ(buff[2], 3);
  }
  {
    sut const orig = {value_t(1), value_t(2), value_t(3)};
    sut cpy{orig};
    EXPECT_EQ(cpy, orig);
  }
  {
    sut const ref = {value_t(1), value_t(2), value_t(3)};
    sut orig{ref};
    sut cpy{std::move(orig)};
    EXPECT_EQ(cpy, ref);
    EXPECT_TRUE(orig.empty());
  }
  {
    sut const orig = {value_t(1), value_t(2), value_t(3)};
    sut cpy0;
    sut cpy1 = {value_t(4)};
    sut cpy2 = {value_t(5), value_t(6), value_t(7)};
    sut cpy3 = {value_t(8), value_t(9), value_t(10), value_t(11)};

    cpy0 = orig;
    EXPECT_EQ(cpy0, orig);
    cpy1 = orig;
    EXPECT_EQ(cpy1, orig);
    cpy2 = orig;
    EXPECT_EQ(cpy2, orig);
    cpy3 = orig;
    EXPECT_EQ(cpy3, orig);
    cpy3 = cpy3;
    EXPECT_EQ(cpy3, orig);
  }
  {
    sut lhs = {value_t(1), value_t(2), value_t(3)};
    sut rhs(5);
    rhs.emplace_back(1);
    rhs.emplace_back(2);
    rhs.emplace_back(3);
    EXPECT_EQ(lhs, rhs);
    rhs.emplace_back(4);
    EXPECT_NE(lhs, rhs);
  }
}
TEST(dynamic_array, mixed_buffer)
{
  enum : std::size_t
  {
    stack_size = 7
  };
  using sut = mixed_buffer<test::resource, stack_size>;
  using value_t = typename sut::value_type;
  {
    sut buff;
    EXPECT_TRUE(buff.empty());
    EXPECT_EQ(buff.capacity(), stack_size);
  }
  {
    sut buff_s(stack_size - 1);
    EXPECT_TRUE(buff_s.empty());
    EXPECT_EQ(buff_s.capacity(), stack_size);

    sut buff_h(stack_size + 1);
    EXPECT_TRUE(buff_h.empty());
    EXPECT_EQ(buff_h.capacity(), stack_size + 1);
  }
  {
    sut buff_s = {value_t(1), value_t(2), value_t(3), value_t(4), value_t(5)};
    EXPECT_EQ(buff_s.size(), 5u);
    EXPECT_EQ(buff_s.capacity(), stack_size);
    EXPECT_EQ(buff_s[0], 1);
    EXPECT_EQ(buff_s[1], 2);
    EXPECT_EQ(buff_s[2], 3);
    EXPECT_EQ(buff_s[3], 4);
    EXPECT_EQ(buff_s[4], 5);

    sut buff_h = {value_t(1), value_t(2), value_t(3), value_t(4),
                  value_t(5), value_t(6), value_t(7), value_t(8)};
    EXPECT_EQ(buff_h.size(), 8u);
    EXPECT_EQ(buff_h.capacity(), 8u);
    EXPECT_EQ(buff_h[0], 1);
    EXPECT_EQ(buff_h[1], 2);
    EXPECT_EQ(buff_h[2], 3);
    EXPECT_EQ(buff_h[3], 4);
    EXPECT_EQ(buff_h[4], 5);
    EXPECT_EQ(buff_h[5], 6);
    EXPECT_EQ(buff_h[6], 7);
    EXPECT_EQ(buff_h[7], 8);
  }
  {
    sut const orig_s = {value_t(1), value_t(2), value_t(3)};
    sut cpy_s{orig_s};
    EXPECT_EQ(cpy_s, orig_s);

    sut const orig_h = {value_t(1), value_t(2), value_t(3), value_t(4),
                        value_t(5), value_t(6), value_t(7), value_t(8)};
    sut cpy_h{orig_h};
    EXPECT_EQ(cpy_h, orig_h);
  }
  {
    sut const ref_s = {value_t(1), value_t(2), value_t(3)};
    sut orig_s{ref_s};
    sut cpy_s{std::move(orig_s)};
    EXPECT_EQ(cpy_s, ref_s);
    EXPECT_EQ(orig_s.capacity(), stack_size);
    EXPECT_EQ(orig_s.size(), ref_s.size());

    sut const ref_h = {value_t(1), value_t(2), value_t(3), value_t(4),
                       value_t(5), value_t(6), value_t(7), value_t(8)};
    sut orig_h{ref_h};
    EXPECT_GT(orig_h.capacity(), stack_size);
    sut cpy_h{std::move(orig_h)};
    EXPECT_EQ(cpy_h, ref_h);
    EXPECT_EQ(orig_h.capacity(), stack_size);
    EXPECT_TRUE(orig_h.empty());
  }
  auto const make_sut = [](sut::size_type S) {
    sut ret(S);
    for (auto idx = 0u; idx < S; ++idx) {
      ret.emplace_back(static_cast<int>(idx));
    }
    return ret;
  };
  {
    sut const orig_s = {value_t(100), value_t(101), value_t(102)};
    sut cpy0;
    sut cpy1 = make_sut(1);
    sut cpy2 = make_sut(3);
    sut cpy3 = make_sut(6);
    sut cpy4 = make_sut(8);
    sut cpy5 = make_sut(9);

    cpy0 = orig_s;
    EXPECT_EQ(cpy0, orig_s);
    cpy1 = orig_s;
    EXPECT_EQ(cpy1, orig_s);
    cpy2 = orig_s;
    EXPECT_EQ(cpy2, orig_s);
    cpy3 = orig_s;
    EXPECT_EQ(cpy3, orig_s);
    cpy4 = orig_s;
    EXPECT_EQ(cpy4, orig_s);
    cpy5 = orig_s;
    EXPECT_EQ(cpy5, orig_s);

    cpy5 = cpy5;
    EXPECT_EQ(cpy5, orig_s);
  }
  {
    sut const orig_s = {value_t(100), value_t(101), value_t(102), value_t(103),
                        value_t(104), value_t(105), value_t(106), value_t(107)};
    sut cpy0;
    sut cpy1 = make_sut(1);
    sut cpy2 = make_sut(3);
    sut cpy3 = make_sut(6);
    sut cpy4 = make_sut(8);
    sut cpy5 = make_sut(9);

    cpy0 = orig_s;
    EXPECT_EQ(cpy0, orig_s);
    cpy1 = orig_s;
    EXPECT_EQ(cpy1, orig_s);
    cpy2 = orig_s;
    EXPECT_EQ(cpy2, orig_s);
    cpy3 = orig_s;
    EXPECT_EQ(cpy3, orig_s);
    cpy4 = orig_s;
    EXPECT_EQ(cpy4, orig_s);
    cpy5 = orig_s;
    EXPECT_EQ(cpy5, orig_s);

    cpy5 = cpy5;
    EXPECT_EQ(cpy5, orig_s);
  }

  {
    sut lhs = {value_t(1), value_t(2), value_t(3)};
    sut rhs(5);
    rhs.emplace_back(1);
    rhs.emplace_back(2);
    rhs.emplace_back(3);
    EXPECT_EQ(lhs, rhs);
    rhs.emplace_back(4);
    EXPECT_NE(lhs, rhs);
  }
}
}
