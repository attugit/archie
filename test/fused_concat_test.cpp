#include <archie/utils/fused/concat.h>
#include <archie/utils/test.h>

namespace fused = archie::utils::fused;

void canConcatTuple() {
  auto a = fused::make_tuple(1, 2u, '3');
  auto x = fused::concat(a, 4.0);

  EXPECT_EQ(4u, fused::tuple_size<decltype(x)>::value);
  EXPECT_EQ(1, fused::get<0>(x));
  EXPECT_EQ(2u, fused::get<1>(x));
  EXPECT_EQ('3', fused::get<2>(x));
  EXPECT_EQ(4.0, fused::get<3>(x));
}

int main() {
  canConcatTuple();
  return 0;
}
