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
  {
    auto a = std::make_unique<unsigned>(1u);
    auto& x = fused::front(a, 2.0, '3');
    static_assert(std::is_same<decltype(x), uptr&>::value, "");
    EXPECT_TRUE(x != nullptr);
    EXPECT_EQ(&a, &x);
  }
}

void canUseBackWithLValue() {
  {
    auto a = 1u;
    auto x = fused::front(a, 2.0, '3');
    static_assert(std::is_same<decltype(x), unsigned>::value, "");
    EXPECT_EQ(1u, x);
    EXPECT_NE(&a, &x);
  }
  {
    auto const a = 1u;
    auto x = fused::front(a, 2.0, '3');
    static_assert(std::is_same<decltype(x), unsigned>::value, "");
    EXPECT_EQ(1u, x);
    EXPECT_NE(&a, &x);
  }
  {
    auto a = 1u;
    auto& x = fused::front(a, 2.0, '3');
    static_assert(std::is_same<decltype(x), unsigned&>::value, "");
    EXPECT_EQ(&a, &x);
  }
  {
    auto const a = 1u;
    auto& x = fused::front(a, 2.0, '3');
    static_assert(std::is_same<decltype(x), unsigned const&>::value, "");
    EXPECT_EQ(&a, &x);
  }
  {
    auto a = 1u;
    auto const& x = fused::front(a, 2.0, '3');
    static_assert(std::is_same<decltype(x), unsigned const&>::value, "");
    EXPECT_EQ(&a, &x);
  }
}

int main() {
  canUseFrontWithRValue();
  canPassNoncopyableToFront();
  canUseBackWithLValue();
  return 0;
}
