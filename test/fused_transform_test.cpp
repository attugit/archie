#include <archie/utils/fused/transform.h>
#include <type_traits>
#include <archie/utils/test.h>

namespace fused = archie::utils::fused;

auto id = [](auto const& x) { return x; };

void canUseTransformWithArgsWithoutTypeChange() {
  auto ret = fused::transform(id, 1, 2, 3);
  static_assert(std::is_same<decltype(ret), fused::tuple<int, int, int>>::value,
                "");
  EXPECT_EQ(1, fused::get<0>(ret));
  EXPECT_EQ(2, fused::get<1>(ret));
  EXPECT_EQ(3, fused::get<2>(ret));
}

void canUseTransformWithTupleWithoutTypeChange() {
  auto t3 = fused::make_tuple(1, 2, 3);
  auto ret = fused::transform(id, t3);
  static_assert(std::is_same<decltype(ret), fused::tuple<int, int, int>>::value,
                "");

  EXPECT_EQ(1, fused::get<0>(ret));
  EXPECT_EQ(2, fused::get<1>(ret));
  EXPECT_EQ(3, fused::get<2>(ret));
}

int main() {
  canUseTransformWithArgsWithoutTypeChange();
  canUseTransformWithTupleWithoutTypeChange();
  return 0;
}
