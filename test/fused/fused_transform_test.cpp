#include <archie/utils/fused/transform.h>
#include <type_traits>
#include <catch.hpp>

namespace {
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

TEST_CASE("canUseTransformWithArgsWithoutTypeChange", "[fused::transform]") {
  auto ret = fused::transform(id, 1, 2, 3);
  static_assert(std::is_same<decltype(ret), fused::tuple<int, int, int>>::value, "");
  REQUIRE(1 == fused::get<0>(ret));
  REQUIRE(2 == fused::get<1>(ret));
  REQUIRE(3 == fused::get<2>(ret));
}

TEST_CASE("canUseTransformWithTupleWithoutTypeChange", "[fused::transform]") {
  auto t3 = fused::make_tuple(1, 2, 3);
  auto ret = fused::transform(id, t3);
  static_assert(std::is_same<decltype(ret), fused::tuple<int, int, int>>::value, "");

  REQUIRE(1 == fused::get<0>(ret));
  REQUIRE(2 == fused::get<1>(ret));
  REQUIRE(3 == fused::get<2>(ret));
}

TEST_CASE("canUseTransformView", "[fused::transform]") {
// FIXME -Werror=conversion
#if 0
  auto t = fused::make_tuple(1u, 2.0, '3');
  auto twice = [](auto&& x) -> decltype(x) {
    x += x;
    return x;
  };
  auto ret = fused::transform_view(twice, t);

  EXPECT_EQ(&fused::get<0>(t), &fused::get<0>(ret).get());
  EXPECT_EQ(&fused::get<1>(t), &fused::get<1>(ret).get());
  EXPECT_EQ(&fused::get<2>(t), &fused::get<2>(ret).get());
#endif
}

TEST_CASE("canUseTransformWithArgsWithTypeChange", "[fused::transform]") {
  auto ret = fused::transform(make_tag, 1, 2u, '3');
  static_assert(
      std::is_same<decltype(ret), fused::tuple<tag<int>, tag<unsigned>, tag<char>>>::value, "");
}

TEST_CASE("canUseTransformWithTupleWithTypeChange", "[fused::transform]") {
  auto t3 = fused::make_tuple(1, 2u, '3');
  auto ret = fused::transform(make_tag, t3);
  static_assert(
      std::is_same<decltype(ret), fused::tuple<tag<int>, tag<unsigned>, tag<char>>>::value, "");
}

TEST_CASE("canUseTransformWithRValueTupleWithTypeChange", "[fused::transform]") {
  auto ret = fused::transform(make_tag, fused::make_tuple(1, 2u, '3'));
  static_assert(
      std::is_same<decltype(ret), fused::tuple<tag<int>, tag<unsigned>, tag<char>>>::value, "");
}

auto create = [](auto&& x) { return x.construct(); };

TEST_CASE("canUseTransformWithTupleToCallElemntsMemberFunctions", "[fused::transform]") {
  auto defs = fused::transform(create, fused::tuple<tag<int>, tag<unsigned>, tag<char>>{});
  static_assert(std::is_same<decltype(defs), fused::tuple<int, unsigned, char>>::value, "");
}
}
