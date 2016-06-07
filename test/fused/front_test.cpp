#include <archie/fused/front.hpp>
#include <archie/fused/tuple.hpp>
#include <memory>
#include <type_traits>
#include <gtest/gtest.h>

namespace {
namespace fused = archie::fused;
using uptr = std::unique_ptr<unsigned>;

TEST(front, canUseFrontWithRValue)
{
  auto a = fused::front(1u, 2.0, '3');
  static_assert(std::is_same<decltype(a), unsigned>::value, "");
  EXPECT_EQ(1u, a);
}

TEST(front, canPassNoncopyableToFront)
{
  {
    auto a = fused::front(1u, 2.0, std::make_unique<char>('c'));
    static_assert(std::is_same<decltype(a), unsigned>::value, "");
    EXPECT_EQ(1u, a);
  }
  {
    auto a = fused::front(std::make_unique<unsigned>(1u), 2.0, '3');
    static_assert(std::is_same<decltype(a), uptr>::value, "");
    EXPECT_NE(a, nullptr);
    EXPECT_EQ(1u, *a);
  }
  {
    auto a = std::make_unique<unsigned>(1u);
    auto& x = fused::front(a, 2.0, '3');
    static_assert(std::is_same<decltype(x), uptr&>::value, "");
    EXPECT_NE(x, nullptr);
    EXPECT_EQ(&a, &x);
  }
}
TEST(front, canUseFrontWithLValue)
{
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

template <typename Tp>
decltype(auto) foo(Tp&& t)
{
  return fused::front(fused::get<0>(std::forward<Tp>(t)));
}

template <typename... Ts>
auto goo(Ts&&... ts)
{
  return foo(fused::make_tuple(std::forward<Ts>(ts)...));
}

TEST(front, canWrapFusedFrontWithFunction)
{
  {
    auto x = foo(fused::make_tuple(1u, 2.0, '3'));
    static_assert(std::is_same<decltype(x), unsigned>::value, "");
    EXPECT_EQ(1u, x);
  }
  {
    auto a = fused::make_tuple(1u, 2.0, '3');
    auto const& x = foo(a);
    static_assert(std::is_same<decltype(x), unsigned const&>::value, "");
    EXPECT_EQ(&fused::get<0>(a), &x);
  }
  {
    auto x = goo(1u, 2.0, '3');
    static_assert(std::is_same<decltype(x), unsigned>::value, "");
    EXPECT_EQ(1u, x);
  }
}
}
