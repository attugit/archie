#include <archie/utils/fused/type_tag.h>
#include <archie/utils/fused/transform.h>
#include <archie/utils/test.h>

namespace fused = archie::utils::fused;

void canCreateTags() {
  auto t3 = fused::make_tuple(1, 2u, '3');
  auto tags = fused::transform(fused::make_tag, t3);
  static_assert(
      std::is_same<decltype(tags), fused::tuple<fused::type_tag<int>, fused::type_tag<unsigned>,
                                                fused::type_tag<char>>>::value,
      "");
}

void canExpandTags() {
  auto expand = [](auto x) { return x.construct(static_cast<typename decltype(x)::type>(0)); };
  auto tags = fused::transform(fused::make_tag, 1, 2u, '3');
  auto defs = fused::transform(expand, tags);
  static_assert(std::is_same<decltype(defs), fused::tuple<int, unsigned, char>>::value, "");
  EXPECT_EQ(0, fused::get<0>(defs));
  EXPECT_EQ(0, fused::get<1>(defs));
  EXPECT_EQ(0, fused::get<2>(defs));
}

void canUseVariableTemplate() {
  auto x = fused::id<int>(1);
  static_assert(std::is_same<decltype(x), int>::value, "");
  EXPECT_EQ(1, x);
}

void canCompareTags() {
  constexpr auto const& x = fused::id<int>;
  constexpr auto const& y = fused::id<unsigned>;
  EXPECT_EQ(x, fused::make_tag(1));
  EXPECT_EQ(y, fused::make_tag(1u));
  EXPECT_NE(x, y);
  EXPECT_NE(fused::make_tag(1), fused::make_tag(1u));
}

int main() {
  canCreateTags();
  canExpandTags();
  canUseVariableTemplate();
  canCompareTags();
  return 0;
}
