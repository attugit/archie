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

int main() {
  canZipTwoTuples();
  return 0;
}
