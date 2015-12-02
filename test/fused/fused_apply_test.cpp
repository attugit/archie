#include <archie/utils/fused/apply.h>
#include <archie/utils/fused/front.h>
#include <catch.hpp>

namespace {
auto count = [](auto const&... xs) { return sizeof...(xs); };

namespace fused = archie::utils::fused;

TEST_CASE("canUseApplyWithRegularArgs", "[fused::apply]") {
  REQUIRE(1u == fused::apply(count, 3));
  REQUIRE(2u == fused::apply(count, 3, 2));
  REQUIRE(3u == fused::apply(count, 3, 2, 1));
}

TEST_CASE("canUseApplyWithTuple", "[fused::apply]") {
  auto t1 = fused::make_tuple(3);
  auto t2 = fused::make_tuple(3, 2);
  auto t3 = fused::make_tuple(3, 2, 1);

  REQUIRE(1u == fused::apply(count, t1));
  REQUIRE(2u == fused::apply(count, t2));
  REQUIRE(3u == fused::apply(count, t3));
}

TEST_CASE("canUseApplyWithRValueTuple", "[fused::apply]") {
  REQUIRE(1u == fused::apply(count, fused::make_tuple(3)));
  REQUIRE(2u == fused::apply(count, fused::make_tuple(3, 2)));
  REQUIRE(3u == fused::apply(count, fused::make_tuple(3, 2, 1)));
}

TEST_CASE("canUseApplyWithMakeTuple", "[fused::apply]") {
  auto x = fused::apply(fused::make_tuple, 1, 2u, '3');
  static_assert(fused::tuple_size(fused::id<decltype(x)>) == 3u, "");
}

TEST_CASE("canUseApplyResultOfAnotherApply", "[fused::apply]") {
  {
    auto x = fused::apply(fused::front, fused::apply(fused::make_tuple, 1, 2u, '3'));
    REQUIRE(1 == x);
  }
  auto a = 1;
  auto b = 2u;
  auto c = '3';
  {
    auto x = fused::apply(fused::front, fused::apply(fused::make_tuple, a, b, c));
    REQUIRE(a == x);
  }
  {
    auto const& x = fused::apply(fused::front, fused::apply(fused::tie, a, b, c));
    REQUIRE(&a == &x);
  }
}
}
