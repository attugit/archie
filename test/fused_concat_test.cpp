#include <archie/utils/fused/concat.h>
#include <archie/utils/test.h>

namespace fused = archie::utils::fused;

void canConcatTuple() {
  {
    auto a = fused::make_tuple(1, 2u, '3');
    auto x = fused::concat(a, 4.0);

    EXPECT_EQ(4u, fused::tuple_size_n(x));
    EXPECT_EQ(1, fused::get<0>(x));
    EXPECT_EQ(2u, fused::get<1>(x));
    EXPECT_EQ('3', fused::get<2>(x));
    EXPECT_EQ(4.0, fused::get<3>(x));
  }
  {
    auto a = fused::make_tuple(1, 2u, '3');
    auto x = fused::concat(a, 4.0, 5, 6u);

    EXPECT_EQ(6u, fused::tuple_size_n(x));
    EXPECT_EQ(1, fused::get<0>(x));
    EXPECT_EQ(2u, fused::get<1>(x));
    EXPECT_EQ('3', fused::get<2>(x));
    EXPECT_EQ(4.0, fused::get<3>(x));
    EXPECT_EQ(5, fused::get<4>(x));
    EXPECT_EQ(6u, fused::get<5>(x));
  }
}

void canConcatTwoTuples() {
  auto a = fused::make_tuple(1, 2u, '3');
  auto b = fused::make_tuple(4.0, 5, 6u);
  auto x = fused::concat(a, b);

  EXPECT_EQ(6u, fused::tuple_size_n(x));
  EXPECT_EQ(1, fused::get<0>(x));
  EXPECT_EQ(2u, fused::get<1>(x));
  EXPECT_EQ('3', fused::get<2>(x));
  EXPECT_EQ(4.0, fused::get<3>(x));
  EXPECT_EQ(5, fused::get<4>(x));
  EXPECT_EQ(6u, fused::get<5>(x));
}

int main() {
  canConcatTuple();
  canConcatTwoTuples();
  return 0;
}
