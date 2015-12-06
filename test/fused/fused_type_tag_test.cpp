#include <archie/fused/type_tag.hpp>
#include <archie/fused/transform.hpp>
#include <catch.hpp>

namespace {
namespace fused = archie::fused;

TEST_CASE("canCreateTags", "[fused::tags]") {
  auto t3 = fused::make_tuple(1, 2u, '3');
  auto tags = fused::transform(fused::make_tag, t3);
  static_assert(
      std::is_same<decltype(tags), fused::tuple<fused::type_tag<int>, fused::type_tag<unsigned>,
                                                fused::type_tag<char>>>::value,
      "");
}

TEST_CASE("canExpandTags", "[fused::tags]") {
  auto expand = [](auto x) { return x.construct(static_cast<typename decltype(x)::type>(0)); };
  auto tags = fused::transform(fused::make_tag, 1, 2u, '3');
  auto defs = fused::transform(expand, tags);
  static_assert(std::is_same<decltype(defs), fused::tuple<int, unsigned, char>>::value, "");
  REQUIRE(0 == fused::get<0>(defs));
  REQUIRE(0 == fused::get<1>(defs));
  REQUIRE(0 == fused::get<2>(defs));
}

TEST_CASE("canUseVariableTemplate", "[fused::tags]") {
  auto x = fused::id<int>(1);
  static_assert(std::is_same<decltype(x), int>::value, "");
  REQUIRE(1 == x);
}

TEST_CASE("canCompareTags", "[fused::tags]") {
  constexpr auto const& x = fused::id<int>;
  constexpr auto const& y = fused::id<unsigned>;
  REQUIRE(x == fused::make_tag(1));
  REQUIRE(y == fused::make_tag(1u));
  REQUIRE(x != y);
  REQUIRE(fused::make_tag(1) != fused::make_tag(1u));
}
}
