#include <archie/utils/fused/tail.h>
#include <archie/utils/fused/tuple.h>
#include <archie/utils/test.h>

namespace fused = archie::utils::fused;

void canGetLValueFusedTupleTail() {
  auto t0 = fused::make_tuple(1, 2u, '3');
  auto x = fused::tail(t0);
  static_assert(fused::tuple_size<decltype(x)>::value == 2u, "");
  EXPECT_EQ(2u, fused::get<0>(x));
  EXPECT_EQ('3', fused::get<1>(x));
}

void canGetRValueFusedTupleTail() {
  auto x = fused::tail(fused::make_tuple(1, 2u, '3'));
  static_assert(fused::tuple_size<decltype(x)>::value == 2u, "");
  EXPECT_EQ(2u, fused::get<0>(x));
  EXPECT_EQ('3', fused::get<1>(x));
}

void canGetConstValueFusedTupleTail() {
  auto const t0 = fused::make_tuple(1, 2u, '3');
  auto x = fused::tail(t0);
  static_assert(fused::tuple_size<decltype(x)>::value == 2u, "");
  EXPECT_EQ(2u, fused::get<0>(x));
  EXPECT_EQ('3', fused::get<1>(x));
}

void canGetFusedTupleOfReferencesTail() {
  auto a = 1;
  auto b = 2u;
  auto c = '3';
  auto t0 = fused::tie(a, b, c);
  auto x = fused::tail(t0);
  static_assert(fused::tuple_size<decltype(x)>::value == 2u, "");
  EXPECT_EQ(&b, &fused::get<0>(x));
  EXPECT_EQ(&c, &fused::get<1>(x));
}

int main() {
  canGetLValueFusedTupleTail();
  canGetRValueFusedTupleTail();
  canGetConstValueFusedTupleTail();
  canGetFusedTupleOfReferencesTail();
  return 0;
}
