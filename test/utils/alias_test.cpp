#include <archie/alias.hpp>
#include <vector>
#include <array>
#include <gtest/gtest.h>
namespace
{
  using namespace archie;

  TEST(alias, alias)
  {
    {
      int x = 0;
      int y = 1;
      auto a = alias(x);
      EXPECT_EQ(unwrap(a), 0);
      a = 2;
      EXPECT_EQ(x, 2);
      a = rebind(y);
      a = 3;
      EXPECT_EQ(x, 2);
      EXPECT_EQ(y, 3);
    }
    {
      const int x = 0;
      const int y = 1;
      auto a = alias(x);
      auto b = alias(x);
      EXPECT_EQ(unwrap(a), 0);
      a = rebind(y);
      EXPECT_EQ(unwrap(a), 1);
      a = rebind(b);
      EXPECT_EQ(unwrap(a), 0);
    }
    {
      std::array<int, 3> array = {{3, 1, 2}};
      std::vector<alias_t<int>> vec;
      vec.emplace_back(array[1]);
      vec.emplace_back(array[2]);
      vec.emplace_back(array[0]);
      std::sort(std::begin(vec), std::end(vec));
      EXPECT_EQ(unwrap(vec[0]), 3);
      EXPECT_EQ(unwrap(vec[1]), 1);
      EXPECT_EQ(unwrap(vec[2]), 2);
      EXPECT_EQ(array[0], 3);
      EXPECT_EQ(array[1], 1);
      EXPECT_EQ(array[2], 2);
      std::sort(std::begin(vec), std::end(vec),
                [](auto const& lhs, auto const& rhs) { return unwrap(lhs) < unwrap(rhs); });
      EXPECT_EQ(unwrap(vec[0]), 1);
      EXPECT_EQ(unwrap(vec[1]), 2);
      EXPECT_EQ(unwrap(vec[2]), 3);
      EXPECT_EQ(array[0], 3);
      EXPECT_EQ(array[1], 1);
      EXPECT_EQ(array[2], 2);
    }
    {
      const int ci = 1;
      int i = 2;
      auto ca = alias(ci);
      EXPECT_EQ(unwrap(ca), 1);
      ca = rebind(i);
      EXPECT_EQ(unwrap(ca), 2);
    }
    {
      int x = 10;
      int y = 12;
      auto foo = [](alias_t<int> a, alias_t<int> b) {
        if (unwrap(a) < unwrap(b)) {
          auto tmp = a;
          a = rebind(b);
          b = rebind(tmp);
        }
        a = 42;
        b = 7;
      };
      foo(ref(x), ref(y));
      EXPECT_EQ(x, 7);
      EXPECT_EQ(y, 42);
    }
    {
      auto x = 1u;
      auto y = 2u;
      auto a = alias(x);
      EXPECT_EQ(unwrap(a), 1u);
      a = y;
      EXPECT_EQ(unwrap(a), 2u);
      EXPECT_EQ(x, 2u);
      a = x;
      EXPECT_EQ(unwrap(a), 2u);
      a = std::move(x);
      EXPECT_EQ(unwrap(a), 2u);
    }
    {
      struct A {
        int x = 1;
      };
      struct B : A {
        int y = 2;
      };
      B x;
      alias_t<A> const a(x);
      EXPECT_EQ((*a).x, 1);
      alias_t<A> b(alias(x));
      EXPECT_EQ(a->x, 1);
    }
    {
      struct Pos {
        int x, y;
      };
      Pos p, q;
      auto als = alias(p);
      als->x = 1;
      als->y = 2;
      EXPECT_EQ(p.x, 1);
      EXPECT_EQ(p.y, 2);
      auto other = alias(p);
      EXPECT_EQ(als, other);
      EXPECT_EQ(als, other);
      auto aq = alias(q);
      if (std::addressof(p) > std::addressof(q)) {
        EXPECT_GT(als, aq);
        EXPECT_GE(als, other);
        EXPECT_GE(als, aq);
        EXPECT_LT(aq, als);
        EXPECT_LE(other, als);
        EXPECT_LE(aq, als);
      }
    }
  }
}
