#include <archie/fused/tuple.hpp>
#include <archie/fused/algorithm.hpp>
#include <functional>
#include <catch.hpp>

namespace {
namespace fused = archie::fused;

auto const plus = std::plus<>{};
auto const ge = std::greater_equal<>{};
auto const le = std::less_equal<>{};
auto const is_odd = [](auto x) { return x % 2 != 0; };

TEST_CASE("canUseAccumulate", "[fused::fold_algo]") {
  REQUIRE(1u == fused::accumulate(plus)(0, 1u));
  REQUIRE(3.0 == fused::accumulate(plus)(0, 1u, 2.0));
  REQUIRE(54.0 == fused::accumulate(plus)(0, 1u, 2.0, '3'));
}

TEST_CASE("canApplyAccumulate", "[fused::fold_algo]") {
  auto const t = fused::make_tuple(0, 1, 2);
  REQUIRE(3 == fused::apply(fused::accumulate(plus), t));
  REQUIRE(6 == fused::apply(fused::accumulate(plus), fused::make_tuple(0, 1, 2, 3)));
}

TEST_CASE("canUseAccumulateWithCustomFunctionObject", "[fused::fold_algo]") {
  auto const t = fused::make_tuple(0, 1, 2);
  auto const f = [](auto a, auto b) { return a + 3 * b; };
  REQUIRE(9 == fused::apply(fused::accumulate(f), t));
  REQUIRE(9 == fused::apply(fused::accumulate(f, 0), t));
  REQUIRE(10 == fused::apply(fused::accumulate(f, 1), t));
}

TEST_CASE("canUseMax", "[fused::fold_algo]") {
  REQUIRE(1.0 == fused::max(0, 1.0));
  REQUIRE('1' == fused::max(0, '1', 2.0));
  REQUIRE(3 == fused::max(0, 3, 2.0));
}

TEST_CASE("canApplyMax", "[fused::fold_algo]") {
  auto const t = fused::make_tuple(0, 3, 2);
  REQUIRE(3 == fused::apply(fused::max, t));
  REQUIRE(4 == fused::apply(fused::max, fused::make_tuple(4, 3, 2)));
  auto const max5 = fused::extremum(ge, 5);
  REQUIRE(5 == fused::apply(max5, fused::make_tuple(4, 3, 2)));
}

TEST_CASE("canApplyMaxWithCustomFunctionObject", "[fused::fold_algo]") {
  auto const t = fused::make_tuple(0, 3, 4);
  auto const f = [](auto const& a, auto const& b) { return (2 * a) >= b; };
  auto const max = fused::extremum(f);
  REQUIRE(3 == fused::apply(max, t));
  REQUIRE(4 == fused::apply(max, fused::make_tuple(4, 3, 6)));
}

TEST_CASE("canUseMin", "[fused::fold_algo]") {
  REQUIRE(0 == fused::min(0, 1));
  REQUIRE(0 == fused::min(2, 0, 1));
  REQUIRE(0 == fused::min(3, 2, 0));
}

TEST_CASE("canApplyMin", "[fused::fold_algo]") {
  auto const t = fused::make_tuple(3, 0, 2);
  REQUIRE(0 == fused::apply(fused::min, t));
  REQUIRE(0 == fused::apply(fused::min, fused::make_tuple(0, 3, 2)));
  auto const min2 = fused::extremum(le, 2);
  REQUIRE(2 == fused::apply(min2, fused::make_tuple(4, 3, 5)));
}

TEST_CASE("canApplyMinWithCustomFunctionObject", "[fused::fold_algo]") {
  auto const t = fused::make_tuple(4, 1, 0);
  auto const f = [](auto a, auto b) { return (a - 2) <= b; };
  auto const min = fused::extremum(f);
  REQUIRE(1 == fused::apply(min, t));
  REQUIRE(4 == fused::apply(min, fused::make_tuple(4, 3, 2)));
}

TEST_CASE("canUseAllOf", "[fused::fold_algo]") {
  REQUIRE(fused::all_of(is_odd)(1, 3, 5));
  REQUIRE_FALSE(fused::all_of(is_odd)(1, 2, 5));
  REQUIRE_FALSE(fused::all_of(is_odd)(2, 2, 2));
}

TEST_CASE("canApplyAllOf", "[fused::fold_algo]") {
  auto const odds = fused::make_tuple(1, 3, 5);
  auto const mixed = fused::make_tuple(1, 2, 3);
  auto const evens = fused::make_tuple(2, 4, 6);
  REQUIRE(fused::apply(fused::all_of(is_odd), odds));
  REQUIRE_FALSE(fused::apply(fused::all_of(is_odd), mixed));
  REQUIRE_FALSE(fused::apply(fused::all_of(is_odd), evens));
}

TEST_CASE("canUseAnyOf", "[fused::fold_algo]") {
  REQUIRE(fused::any_of(is_odd)(1, 3, 5));
  REQUIRE(fused::any_of(is_odd)(1, 2, 5));
  REQUIRE_FALSE(fused::any_of(is_odd)(2, 2, 2));
}

TEST_CASE("canApplyAnyOf", "[fused::fold_algo]") {
  auto const odds = fused::make_tuple(1, 3, 5);
  auto const mixed = fused::make_tuple(1, 2, 3);
  auto const evens = fused::make_tuple(2, 4, 6);
  REQUIRE(fused::apply(fused::any_of(is_odd), odds));
  REQUIRE(fused::apply(fused::any_of(is_odd), mixed));
  REQUIRE_FALSE(fused::apply(fused::any_of(is_odd), evens));
}

TEST_CASE("canUseNoneOf", "[fused::fold_algo]") {
  REQUIRE_FALSE(fused::none_of(is_odd)(1, 3, 5));
  REQUIRE_FALSE(fused::none_of(is_odd)(1, 2, 5));
  REQUIRE(fused::none_of(is_odd)(2, 2, 2));
}

TEST_CASE("canApplyNoneOf", "[fused::fold_algo]") {
  auto const odds = fused::make_tuple(1, 3, 5);
  auto const mixed = fused::make_tuple(1, 2, 3);
  auto const evens = fused::make_tuple(2, 4, 6);
  REQUIRE_FALSE(fused::apply(fused::none_of(is_odd), odds));
  REQUIRE_FALSE(fused::apply(fused::none_of(is_odd), mixed));
  REQUIRE(fused::apply(fused::none_of(is_odd), evens));
}

TEST_CASE("canUseCountIf", "[fused::fold_algo]") {
  REQUIRE(0 == fused::count_if(is_odd)(0));
  REQUIRE(1 == fused::count_if(is_odd)(1));
  REQUIRE(1 == fused::count_if(is_odd)(0, 1, 2));
  REQUIRE(2 == fused::count_if(is_odd)(0, 1, 2, 3));
}

TEST_CASE("canApplyCountIf", "[fused::fold_algo]") {
  REQUIRE(0 == fused::apply(fused::count_if(is_odd), fused::make_tuple(0)));
  REQUIRE(1 == fused::apply(fused::count_if(is_odd), fused::make_tuple(1)));
  REQUIRE(1 == fused::apply(fused::count_if(is_odd), fused::make_tuple(0, 1, 2)));
  REQUIRE(2 == fused::apply(fused::count_if(is_odd), fused::make_tuple(0, 1, 2, 3)));
}

TEST_CASE("canUseIsSorted", "[fused::fold_algo]") {
  REQUIRE(fused::is_sorted(le)(0));
  REQUIRE(fused::is_sorted(le)(0, 1u));
  REQUIRE(fused::is_sorted(le)(0, 1u, 2.0));
  REQUIRE(fused::is_sorted(le)(0, 1u, 2.0, '3'));
  REQUIRE_FALSE(fused::is_sorted(le)(0, 1u, 2.0, 0));
  REQUIRE_FALSE(fused::is_sorted(le)(0, 1u, 0.0, '3'));
}

TEST_CASE("canApplyIsSorted", "[fused::fold_algo]") {
  REQUIRE(fused::apply(fused::is_sorted(le), fused::make_tuple(0)));
  REQUIRE(fused::apply(fused::is_sorted(le), fused::make_tuple(0, 1u)));
  REQUIRE(fused::apply(fused::is_sorted(le), fused::make_tuple(0, 1u, 2.0)));
  REQUIRE(fused::apply(fused::is_sorted(le), fused::make_tuple(0, 1u, 2.0, '3')));
  REQUIRE_FALSE(fused::apply(fused::is_sorted(le), fused::make_tuple(0, 1u, 2.0, 0)));
  REQUIRE_FALSE(fused::apply(fused::is_sorted(le), fused::make_tuple(0, 1u, 0.0, '3')));
}
}
