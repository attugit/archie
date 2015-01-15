#include <archie/utils/fused/find.h>
#include <archie/utils/test.h>
#include <type_traits>

namespace fused = archie::utils::fused;

void canUseFusedFind() {
  unsigned a = 0u;
  int b = 1;
  char c = '2';
  double d = 3.0;
  unsigned e = 4u;

  auto x = fused::find<unsigned&>(a, b, c, d, e);
  auto y = fused::find<double&>(a, b, c, d, e);
  EXPECT_EQ(a, x);
  EXPECT_EQ(d, y);

  EXPECT_EQ(1, fused::find<int>(1, 2u, 3.0, '4'));
  EXPECT_EQ(2u, fused::find<unsigned>(1, 2u, 3.0, '4'));
  EXPECT_EQ(3.0, fused::find<double>(1, 2u, 3.0, '4'));
  EXPECT_EQ('4', fused::find<char>(1, 2u, 3.0, '4'));
}

template <typename Tp>
using is_u = std::is_unsigned<std::decay_t<Tp>>;

template <typename Tp>
using is_s = std::is_signed<std::decay_t<Tp>>;

void canUseFusedFindIf() {
  unsigned a = 0u;
  int b = 1;
  char c = '2';
  double d = 3.0;
  unsigned e = 4u;

  auto x = fused::find_if<is_u>(a, b, c, d, e);
  auto y = fused::find_if<is_s>(a, b, c, d, e);
  EXPECT_EQ(a, x);
  EXPECT_EQ(b, y);
}

int main() {
  canUseFusedFind();
  canUseFusedFindIf();
  return 0;
}
