#include <archie/fused/conditional.hpp>
#include <catch.hpp>

namespace {
struct goo {
  int operator()() const { return 3; }
};
struct hoo {
  int operator()(int) const { return 4; }
};

namespace fused = archie::fused;

TEST_CASE("canUseConditionalT", "[fused::conditional]")
{
  fused::conditional_t<goo, hoo> cond;
  auto x = cond();
  auto y = cond(1);
  REQUIRE(3 == x);
  REQUIRE(4 == y);
}

TEST_CASE("canUseConditional", "[fused::conditional]")
{
  auto x = fused::conditional<goo, hoo>();
  auto y = fused::conditional<goo, hoo>(1);
  REQUIRE(3 == x);
  REQUIRE(4 == y);
}

TEST_CASE("canUseMakeConditional", "[fused::conditional]")
{
  constexpr auto f = fused::make_conditional(goo{}, hoo{});
  REQUIRE(3 == f());
  REQUIRE(4 == f(7));
}

TEST_CASE("canUseConditionalWithLambda", "[fused::conditional]")
{
  auto const f = [](int i) { return i + 2; };
  auto const g = [](int i, int j) { return 2 * i + j; };
  auto const h = [](int i, int j, int k) { return i * j + k; };
  auto const c = fused::make_conditional(f);
  auto const d = fused::make_conditional(f, g);
  auto const e = fused::make_conditional(f, g, h);
  REQUIRE(f(1) == c(1));
  REQUIRE(f(3) == c(3));
  REQUIRE(f(1) == d(1));
  REQUIRE(f(3) == d(3));
  REQUIRE(g(1, 1) == d(1, 1));
  REQUIRE(g(3, 2) == d(3, 2));
  REQUIRE(f(1) == e(1));
  REQUIRE(f(3) == e(3));
  REQUIRE(g(1, 1) == e(1, 1));
  REQUIRE(g(3, 2) == e(3, 2));
  REQUIRE(h(1, 1, 2) == e(1, 1, 2));
  REQUIRE(h(3, 2, 3) == e(3, 2, 3));
}
}
