#include <archie/utils/fused/fold.h>
#include <archie/utils/fused/tuple.h>
#include <archie/utils/fused/apply.h>
#include <catch.hpp>

namespace {
namespace fused = archie::utils::fused;

TEST_CASE("canUseFold", "[fused::fold]") {
  auto const mkt = [](auto&& s, auto&& e) { return fused::make_tuple(s, e); };
  REQUIRE(fused::make_tuple(0, 1) == fused::fold(mkt, 0, 1));
  REQUIRE(fused::make_tuple(fused::make_tuple(0, 1), 2) == fused::fold(mkt, 0, 1, 2));
}

TEST_CASE("canApplyFold", "[fused::fold]") {
  auto const mkt = [](auto&& s, auto&& e) { return fused::make_tuple(s, e); };
  auto const t = fused::make_tuple(0, 1, 2);
  REQUIRE(fused::make_tuple(fused::make_tuple(0, 1), 2) == fused::apply(fused::make_fold(mkt), t));
}

struct sorted {
  template <typename Tp, typename Up, typename Vp, typename... Rs>
  constexpr bool operator()(Tp const t, Up const& u, Vp const& v, Rs const&...) const {
    return t ? (u < v) : false;
  }
  template <typename Tp, typename Up>
  constexpr bool operator()(Tp const& t, Up) const {
    return t;
  }
};

TEST_CASE("canUseGreedyFold", "[fused::fold]") {
  auto const sf = sorted{};
  REQUIRE(fused::greedy_fold(sf, true, 0, 1));
  REQUIRE(fused::greedy_fold(sf, true, 0, 1, 2));
  REQUIRE(fused::greedy_fold(sf, true, 0, 1, 2, 3));
  REQUIRE_FALSE(fused::greedy_fold(sf, true, 0, 1, 2, 1));
  REQUIRE_FALSE(fused::greedy_fold(sf, true, 0, 1, 0, 3));
}
}
