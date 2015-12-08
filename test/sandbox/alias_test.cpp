#include <archie/alias.hpp>
#include <vector>
#include <array>
#include <catch.hpp>
namespace {
using namespace archie;

TEST_CASE("alias", "[alias]") {
  SECTION("ref") {
    int x = 0;
    int y = 1;
    auto a = alias(x);
    REQUIRE(unwrap(a) == 0);
    a = 2;
    REQUIRE(x == 2);
    a = rebind(y);
    a = 3;
    REQUIRE(x == 2);
    REQUIRE(y == 3);
  }
  SECTION("const ref") {
    const int x = 0;
    const int y = 1;
    auto a = alias(x);
    auto b = alias(x);
    REQUIRE(unwrap(a) == 0);
    a = rebind(y);
    REQUIRE(unwrap(a) == 1);
    a = rebind(b);
    REQUIRE(unwrap(a) == 0);
  }
  SECTION("sorting") {
    std::array<int, 3> array = {{3, 1, 2}};
    std::vector<alias_t<int>> vec;
    vec.emplace_back(array[1]);
    vec.emplace_back(array[2]);
    vec.emplace_back(array[0]);
    std::sort(std::begin(vec), std::end(vec));
    REQUIRE(unwrap(vec[0]) == 3);
    REQUIRE(unwrap(vec[1]) == 1);
    REQUIRE(unwrap(vec[2]) == 2);
    REQUIRE(array[0] == 3);
    REQUIRE(array[1] == 1);
    REQUIRE(array[2] == 2);
    std::sort(std::begin(vec), std::end(vec),
              [](auto const& lhs, auto const& rhs) { return unwrap(lhs) < unwrap(rhs); });
    REQUIRE(unwrap(vec[0]) == 1);
    REQUIRE(unwrap(vec[1]) == 2);
    REQUIRE(unwrap(vec[2]) == 3);
    REQUIRE(array[0] == 3);
    REQUIRE(array[1] == 1);
    REQUIRE(array[2] == 2);
  }
  SECTION("cross-const") {
    const int ci = 1;
    int i = 2;
    auto ca = alias(ci);
    REQUIRE(unwrap(ca) == 1);
    ca = rebind(i);
    REQUIRE(unwrap(ca) == 2);
  }
  SECTION("function arg") {
    int x = 10;
    int y = 12;
    auto foo = [](alias_t<int> a, alias_t<int> b) {
      if (unwrap(a) < unwrap(b)) {
        auto tmp = a;
        a = rebind(b);
        b = rebind(tmp);
      }
      a = 42;
      b = 7;
    };
    foo(ref(x), ref(y));
    REQUIRE(x == 7);
    REQUIRE(y == 42);
  }
  SECTION("assign") {
    auto x = 1u;
    auto y = 2u;
    auto a = alias(x);
    REQUIRE(unwrap(a) == 1u);
    a = y;
    REQUIRE(unwrap(a) == 2u);
    REQUIRE(x == 2u);
    a = x;
    REQUIRE(unwrap(a) == 2u);
    a = std::move(x);
    REQUIRE(unwrap(a) == 2u);
  }
  SECTION("cross-ctor") {
    struct A {
      int x = 1;
    };
    struct B : A {
      int y = 2;
    };
    B x;
    alias_t<A> const a(x);
    REQUIRE((*a).x == 1);
    alias_t<A> b(alias(x));
    REQUIRE(a->x == 1);
  }
  SECTION("deref") {
    struct Pos {
      int x, y;
    };
    Pos p, q;
    auto als = alias(p);
    als->x = 1;
    als->y = 2;
    REQUIRE(p.x == 1);
    REQUIRE(p.y == 2);
    auto other = alias(p);
    REQUIRE(als == other);
    REQUIRE_FALSE(als != other);
    auto aq = alias(q);
    if (std::addressof(p) > std::addressof(q)) {
      REQUIRE(als > aq);
      REQUIRE(als >= other);
      REQUIRE(als >= aq);
      REQUIRE(aq < als);
      REQUIRE(other <= als);
      REQUIRE(aq <= als);
    }
  }
}
}
