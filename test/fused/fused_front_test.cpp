#include <archie/fused/front.h>
#include <archie/fused/tuple.h>
#include <memory>
#include <type_traits>
#include <catch.hpp>

namespace {
namespace fused = archie::fused;
using uptr = std::unique_ptr<unsigned>;

TEST_CASE("canUseFrontWithRValue") {
  auto a = fused::front(1u, 2.0, '3');
  static_assert(std::is_same<decltype(a), unsigned>::value, "");
  REQUIRE(1u == a);
}

TEST_CASE("canPassNoncopyableToFront") {
  {
    auto a = fused::front(1u, 2.0, std::make_unique<char>('c'));
    static_assert(std::is_same<decltype(a), unsigned>::value, "");
    REQUIRE(1u == a);
  }
  {
    auto a = fused::front(std::make_unique<unsigned>(1u), 2.0, '3');
    static_assert(std::is_same<decltype(a), uptr>::value, "");
    REQUIRE(a != nullptr);
    REQUIRE(1u == *a);
  }
  {
    auto a = std::make_unique<unsigned>(1u);
    auto& x = fused::front(a, 2.0, '3');
    static_assert(std::is_same<decltype(x), uptr&>::value, "");
    REQUIRE(x != nullptr);
    REQUIRE(&a == &x);
  }
}
TEST_CASE("canUseFrontWithLValue") {
  {
    auto a = 1u;
    auto x = fused::front(a, 2.0, '3');
    static_assert(std::is_same<decltype(x), unsigned>::value, "");
    REQUIRE(1u == x);
    REQUIRE(&a != &x);
  }
  {
    auto const a = 1u;
    auto x = fused::front(a, 2.0, '3');
    static_assert(std::is_same<decltype(x), unsigned>::value, "");
    REQUIRE(1u == x);
    REQUIRE(&a != &x);
  }
  {
    auto a = 1u;
    auto& x = fused::front(a, 2.0, '3');
    static_assert(std::is_same<decltype(x), unsigned&>::value, "");
    REQUIRE(&a == &x);
  }
  {
    auto const a = 1u;
    auto& x = fused::front(a, 2.0, '3');
    static_assert(std::is_same<decltype(x), unsigned const&>::value, "");
    REQUIRE(&a == &x);
  }
  {
    auto a = 1u;
    auto const& x = fused::front(a, 2.0, '3');
    static_assert(std::is_same<decltype(x), unsigned const&>::value, "");
    REQUIRE(&a == &x);
  }
}

template <typename Tp>
decltype(auto) foo(Tp&& t) {
  return fused::front(fused::get<0>(std::forward<Tp>(t)));
}

template <typename... Ts>
auto goo(Ts&&... ts) {
  return foo(fused::make_tuple(std::forward<Ts>(ts)...));
}

TEST_CASE("canWrapFusedFrontWithFunction") {
  {
    auto x = foo(fused::make_tuple(1u, 2.0, '3'));
    static_assert(std::is_same<decltype(x), unsigned>::value, "");
    REQUIRE(1u == x);
  }
  {
    auto a = fused::make_tuple(1u, 2.0, '3');
    auto const& x = foo(a);
    static_assert(std::is_same<decltype(x), unsigned const&>::value, "");
    REQUIRE(&fused::get<0>(a) == &x);
  }
  {
    auto x = goo(1u, 2.0, '3');
    static_assert(std::is_same<decltype(x), unsigned>::value, "");
    REQUIRE(1u == x);
  }
}
}
