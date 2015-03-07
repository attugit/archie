#include <archie/utils/fused/apply.h>
#include <archie/utils/fused/front.h>

auto count = [](auto const&... xs) { return sizeof...(xs); };

namespace fused = archie::utils::fused;

#include <archie/utils/test.h>

void canUseApplyWithRegularArgs() {
  EXPECT_EQ(1u, fused::apply(count, 3));
  EXPECT_EQ(2u, fused::apply(count, 3, 2));
  EXPECT_EQ(3u, fused::apply(count, 3, 2, 1));
}

void canUseApplyWithTuple() {
  auto t1 = fused::make_tuple(3);
  auto t2 = fused::make_tuple(3, 2);
  auto t3 = fused::make_tuple(3, 2, 1);

  EXPECT_EQ(1u, fused::apply(count, t1));
  EXPECT_EQ(2u, fused::apply(count, t2));
  EXPECT_EQ(3u, fused::apply(count, t3));
}

void canUseApplyWithRValueTuple() {
  EXPECT_EQ(1u, fused::apply(count, fused::make_tuple(3)));
  EXPECT_EQ(2u, fused::apply(count, fused::make_tuple(3, 2)));
  EXPECT_EQ(3u, fused::apply(count, fused::make_tuple(3, 2, 1)));
}

void canUseApplyWithMakeTuple() {
  auto x = fused::apply(fused::make_tuple, 1, 2u, '3');
  static_assert(fused::tuple_size_n(fused::type_tag<decltype(x)>{}) == 3u, "");
}

void canUseApplyResultOfAnotherApply() {
  {
    auto x =
        fused::apply(fused::front, fused::apply(fused::make_tuple, 1, 2u, '3'));
    EXPECT_EQ(1, x);
  }
  auto a = 1;
  auto b = 2u;
  auto c = '3';
  {
    auto x =
        fused::apply(fused::front, fused::apply(fused::make_tuple, a, b, c));
    EXPECT_EQ(a, x);
  }
  {
    auto const& x =
        fused::apply(fused::front, fused::apply(fused::tie, a, b, c));
    EXPECT_EQ(&a, &x);
  }
}

int main() {
  canUseApplyWithRegularArgs();
  canUseApplyWithTuple();
  canUseApplyWithRValueTuple();
  canUseApplyWithMakeTuple();
  canUseApplyResultOfAnotherApply();
  return 0;
}
