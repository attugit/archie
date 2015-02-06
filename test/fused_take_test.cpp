#include <archie/utils/fused/take.h>

template <unsigned>
struct utype {};

using _0 = utype<0>;
using _1 = utype<1>;
namespace fused = archie::utils::fused;

#include <type_traits>

namespace {
using x = decltype(fused::take<1>(fused::tuple<_0, _1>{}));
static_assert(std::is_same<x, fused::tuple<const _0>>::value, "");
using y = decltype(fused::take<2>(fused::tuple<_0, _1>{}));
static_assert(std::is_same<y, fused::tuple<const _0, const _1>>::value, "");
}

#include <archie/utils/test.h>

void canTakeElementsFromTuple() {
  auto a = fused::make_tuple(1, 2.0, '3');
  auto b = fused::take<1>(a);
  auto c = fused::take<2>(a);
  auto d = fused::take<3>(a);

  EXPECT_EQ(1, fused::get<0>(b));
  EXPECT_EQ(1, fused::get<0>(c));
  EXPECT_EQ(2.0, fused::get<1>(c));
  EXPECT_EQ(1, fused::get<0>(d));
  EXPECT_EQ(2.0, fused::get<1>(d));
  EXPECT_EQ('3', fused::get<2>(d));
}

int main() {
  canTakeElementsFromTuple();
  return 0;
}
