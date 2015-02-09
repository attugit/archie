#include <archie/utils/fused/front.h>
#include <archie/utils/test.h>
#include <memory>
#include <type_traits>

namespace fused = archie::utils::fused;
using uptr = std::unique_ptr<unsigned>;

void canUseFrontWithRValue() {
  auto a = fused::front(1u, 2.0, '3');
  static_assert(std::is_same<decltype(a), unsigned>::value, "");
  EXPECT_EQ(1u, a);
}

void canPassNoncopyableToFront() {
  {
    auto a = fused::front(1u, 2.0, std::make_unique<char>('c'));
    static_assert(std::is_same<decltype(a), unsigned>::value, "");
    EXPECT_EQ(1u, a);
  }
  {
    auto a = fused::front(std::make_unique<unsigned>(1u), 2.0, '3');
    static_assert(std::is_same<decltype(a), uptr>::value, "");
    EXPECT_TRUE(a != nullptr);
    EXPECT_EQ(1u, *a);
  }
}

void canUseBackWithLValue() {
  unsigned a = 1u;
  auto const& x = fused::front(a, 2.0, '3');
  EXPECT_EQ(1u, x);
  EXPECT_EQ(&a, &x);
}

int main() {
  canUseFrontWithRValue();
  canPassNoncopyableToFront();
  canUseBackWithLValue();
  return 0;
}
