#include <archie/utils/fused/select_overload.h>
#include <archie/utils/test.h>
#include <utility>

namespace fused = archie::utils::fused;

template <unsigned N, typename = typename std::enable_if<(N <= 8)>::type>
constexpr decltype(auto) foo(fused::choice<1>) {
  return 1u;
}

template <unsigned N, typename = typename std::enable_if<(N <= 16)>::type>
constexpr decltype(auto) foo(fused::choice<2>) {
  return 2u;
}

template <unsigned N>
constexpr decltype(auto) foo(fused::otherwise) {
  return 11u;
}

void canSelectOverload() {
  auto x = foo<8>(fused::select_overload);
  auto y = foo<16>(fused::select_overload);
  auto z = foo<17>(fused::select_overload);

  EXPECT_EQ(1u, x);
  EXPECT_EQ(2u, y);
  EXPECT_EQ(11u, z);
}

int main() {
  canSelectOverload();
  return 0;
}
