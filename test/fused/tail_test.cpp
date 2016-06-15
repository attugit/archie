#include <archie/fused/tail.hpp>
#include <archie/fused/tuple.hpp>
#include <gtest/gtest.h>

namespace
{
  namespace fused = archie::fused;

  TEST(tail, canGetLValueFusedTupleTail)
  {
    auto t0 = fused::make_tuple(1, 2u, '3');
    auto x = fused::tail(t0);
    static_assert(fused::tuple_size(fused::id<decltype(x)>) == 2u, "");
    EXPECT_EQ(2u, fused::get<0>(x));
    EXPECT_EQ('3', fused::get<1>(x));
  }

  TEST(tail, canGetRValueFusedTupleTail)
  {
    auto x = fused::tail(fused::make_tuple(1, 2u, '3'));
    static_assert(fused::tuple_size(fused::id<decltype(x)>) == 2u, "");
    EXPECT_EQ(2u, fused::get<0>(x));
    EXPECT_EQ('3', fused::get<1>(x));
  }

  TEST(tail, canGetConstValueFusedTupleTail)
  {
    auto const t0 = fused::make_tuple(1, 2u, '3');
    auto x = fused::tail(t0);
    static_assert(fused::tuple_size(fused::id<decltype(x)>) == 2u, "");
    EXPECT_EQ(2u, fused::get<0>(x));
    EXPECT_EQ('3', fused::get<1>(x));
  }

  TEST(tail, canGetFusedTupleOfReferencesTail)
  {
    auto a = 1;
    auto b = 2u;
    auto c = '3';
    auto t0 = fused::tie(a, b, c);
    auto x = fused::tail(t0);
    static_assert(fused::tuple_size(fused::id<decltype(x)>) == 2u, "");
    EXPECT_EQ(&b, &fused::get<0>(x));
    EXPECT_EQ(&c, &fused::get<1>(x));
  }
}
