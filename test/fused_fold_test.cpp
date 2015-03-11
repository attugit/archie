#include <archie/utils/fused/fold.h>
#include <archie/utils/fused/tuple.h>
#include <archie/utils/fused/apply.h>
#include <archie/utils/test.h>

namespace fused = archie::utils::fused;

void canUseFold() {
  auto const mkt = [](auto&& s, auto&& e) { return fused::make_tuple(s, e); };
  EXPECT_EQ(fused::make_tuple(0, 1), fused::fold(mkt, 0, 1));
  EXPECT_EQ(fused::make_tuple(fused::make_tuple(0, 1), 2), fused::fold(mkt, 0, 1, 2));
}

void canApplyFold() {
  auto const mkt = [](auto&& s, auto&& e) { return fused::make_tuple(s, e); };
  auto const t = fused::make_tuple(0, 1, 2);
  EXPECT_EQ(fused::make_tuple(fused::make_tuple(0, 1), 2), fused::apply(fused::make_fold(mkt), t));
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

void canUseGreedyFold() {
  auto const sf = sorted{};
  EXPECT_TRUE(fused::greedy_fold(sf, true, 0, 1));
  EXPECT_TRUE(fused::greedy_fold(sf, true, 0, 1, 2));
  EXPECT_TRUE(fused::greedy_fold(sf, true, 0, 1, 2, 3));
  EXPECT_FALSE(fused::greedy_fold(sf, true, 0, 1, 2, 1));
  EXPECT_FALSE(fused::greedy_fold(sf, true, 0, 1, 0, 3));
}

int main() {
  canUseFold();
  canApplyFold();
  canUseGreedyFold();
  return 0;
}
