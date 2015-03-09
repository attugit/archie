#include <archie/utils/fused/transform.h>
#include <type_traits>
#include <archie/utils/test.h>

namespace fused = archie::utils::fused;

template <typename Tp>
struct tag {
  using type = Tp;
  template <typename... Us>
  type construct(Us&&... us) const {
    return type{std::forward<Us>(us)...};
  }
};

auto id = [](auto const& x) { return x; };
auto make_tag = [](auto&& x) { return tag<std::decay_t<decltype(x)>>{}; };

void canUseTransformWithArgsWithoutTypeChange() {
  auto ret = fused::transform(id, 1, 2, 3);
  static_assert(std::is_same<decltype(ret), fused::tuple<int, int, int>>::value, "");
  EXPECT_EQ(1, fused::get<0>(ret));
  EXPECT_EQ(2, fused::get<1>(ret));
  EXPECT_EQ(3, fused::get<2>(ret));
}

void canUseTransformWithTupleWithoutTypeChange() {
  auto t3 = fused::make_tuple(1, 2, 3);
  auto ret = fused::transform(id, t3);
  static_assert(std::is_same<decltype(ret), fused::tuple<int, int, int>>::value, "");

  EXPECT_EQ(1, fused::get<0>(ret));
  EXPECT_EQ(2, fused::get<1>(ret));
  EXPECT_EQ(3, fused::get<2>(ret));
}

void canUseTransformView() {
  auto t = fused::make_tuple(1u, 2.0, '3');
  auto twice = [](auto&& x) -> decltype(x) {
    x += x;
    return x;
  };
  auto ret = fused::transform_view(twice, t);

  EXPECT_EQ(&fused::get<0>(t), &fused::get<0>(ret).get());
  EXPECT_EQ(&fused::get<1>(t), &fused::get<1>(ret).get());
  EXPECT_EQ(&fused::get<2>(t), &fused::get<2>(ret).get());
}

void canUseTransformWithArgsWithTypeChange() {
  auto ret = fused::transform(make_tag, 1, 2u, '3');
  static_assert(
      std::is_same<decltype(ret), fused::tuple<tag<int>, tag<unsigned>, tag<char>>>::value, "");
}

void canUseTransformWithTupleWithTypeChange() {
  auto t3 = fused::make_tuple(1, 2u, '3');
  auto ret = fused::transform(make_tag, t3);
  static_assert(
      std::is_same<decltype(ret), fused::tuple<tag<int>, tag<unsigned>, tag<char>>>::value, "");
}

void canUseTransformWithRValueTupleWithTypeChange() {
  auto ret = fused::transform(make_tag, fused::make_tuple(1, 2u, '3'));
  static_assert(
      std::is_same<decltype(ret), fused::tuple<tag<int>, tag<unsigned>, tag<char>>>::value, "");
}

auto create = [](auto&& x) { return x.construct(); };

void canUseTransformWithTupleToCallElemntsMemberFunctions() {
  auto defs = fused::transform(create, fused::tuple<tag<int>, tag<unsigned>, tag<char>>{});
  static_assert(std::is_same<decltype(defs), fused::tuple<int, unsigned, char>>::value, "");
}

int main() {
  canUseTransformWithArgsWithoutTypeChange();
  canUseTransformWithTupleWithoutTypeChange();
  canUseTransformView();
  canUseTransformWithArgsWithTypeChange();
  canUseTransformWithTupleWithTypeChange();
  canUseTransformWithRValueTupleWithTypeChange();
  canUseTransformWithTupleToCallElemntsMemberFunctions();
  return 0;
}
