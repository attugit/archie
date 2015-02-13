#include <archie/utils/fused/zip.h>
#include <archie/utils/test.h>

namespace fused = archie::utils::fused;

void canZipTwoTuples() {
  auto a = fused::make_tuple(1, 2u, '3');
  auto b = fused::make_tuple('4', 5.0, 6u);
  auto x = fused::zip(a, b);

  EXPECT_EQ(3u, fused::tuple_size<decltype(x)>::value);
  EXPECT_EQ(fused::make_tuple(1, '4'), fused::get<0>(x));
  EXPECT_EQ(fused::make_tuple(2u, 5.0), fused::get<1>(x));
  EXPECT_EQ(fused::make_tuple('3', 6u), fused::get<2>(x));
}

void canZipViewTwoTuples() {
  auto a = fused::make_tuple(1, 2u, '3');
  auto b = fused::make_tuple('4', 5.0, 6u);
  auto x = fused::zip_view(a, b);

  EXPECT_EQ(3u, fused::tuple_size<decltype(x)>::value);
  auto const& x0 = fused::get<0>(x);
  auto const& x1 = fused::get<1>(x);
  auto const& x2 = fused::get<2>(x);

  EXPECT_EQ(&fused::get<0>(a), &fused::get<0>(x0));
  EXPECT_EQ(&fused::get<1>(a), &fused::get<0>(x1));
  EXPECT_EQ(&fused::get<2>(a), &fused::get<0>(x2));

  EXPECT_EQ(&fused::get<0>(b), &fused::get<1>(x0));
  EXPECT_EQ(&fused::get<1>(b), &fused::get<1>(x1));
  EXPECT_EQ(&fused::get<2>(b), &fused::get<1>(x2));
}

int main() {
  canZipTwoTuples();
  canZipViewTwoTuples();
  return 0;
}
