#include <archie/fused/fold.hpp>
#include <archie/fused/tuple.hpp>
#include <archie/fused/apply.hpp>
#include <gtest/gtest.h>

namespace {
namespace fused = archie::fused;

TEST(fold, canUseFold)
{
  auto const mkt = [](auto&& s, auto&& e) { return fused::make_tuple(s, e); };
  EXPECT_EQ(fused::make_tuple(0, 1), fused::fold(mkt, 0, 1));
  EXPECT_EQ(fused::make_tuple(fused::make_tuple(0, 1), 2), fused::fold(mkt, 0, 1, 2));
}

TEST(fold, canApplyFold)
{
  auto const mkt = [](auto&& s, auto&& e) { return fused::make_tuple(s, e); };
  auto const t = fused::make_tuple(0, 1, 2);
  EXPECT_EQ(fused::make_tuple(fused::make_tuple(0, 1), 2), fused::apply(fused::make_fold(mkt), t));
}

struct sorted {
  template <typename Tp, typename Up, typename Vp, typename... Rs>
  constexpr bool operator()(Tp const t, Up const& u, Vp const& v, Rs const&...) const
  {
    return t ? (u < v) : false;
  }
  template <typename Tp, typename Up>
  constexpr bool operator()(Tp const& t, Up) const
  {
    return t;
  }
};

TEST(fold, canUseGreedyFold)
{
  auto const sf = sorted{};
  EXPECT_TRUE(fused::greedy_fold(sf, true, 0, 1));
  EXPECT_TRUE(fused::greedy_fold(sf, true, 0, 1, 2));
  EXPECT_TRUE(fused::greedy_fold(sf, true, 0, 1, 2, 3));
  EXPECT_FALSE(fused::greedy_fold(sf, true, 0, 1, 2, 1));
  EXPECT_FALSE(fused::greedy_fold(sf, true, 0, 1, 0, 3));
}
}
