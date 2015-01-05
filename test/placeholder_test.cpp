#include <archie/utils/fused/placeholder.h>
namespace fused = archie::utils::fused;
#include <archie/utils/test.h>

void canAccessArgumentWithPlaceholder() {
  auto a = 1u;
  auto b = 2.0;
  auto c = '3';

  auto _0 = fused::placeholder<0>{};
  auto _1 = fused::placeholder<1>{};
  auto _2 = fused::placeholder<2>{};

  auto const& x = _0(a, b, c);
  auto const& y = _1(a, b, c);
  auto const& z = _2(a, b, c);

  EXPECT_EQ(&a, &x);
  EXPECT_EQ(&b, &y);
  EXPECT_EQ(&c, &z);
}

int main() {
  canAccessArgumentWithPlaceholder();
  return 0;
}
