#include <archie/meta/functional.hpp>
#include <archie/meta/number.hpp>
#include <catch.hpp>

namespace {
namespace meta = archie::meta;

TEST_CASE("meta::sum")
{
  static_assert(std::is_same<int, meta::if_t<meta::true_t, int, char>>::value, "");
  static_assert(std::is_same<char, meta::if_t<meta::false_t, int, char>>::value, "");

  static_assert(meta::less_t<meta::number<1>, meta::number<2>>::value, "");
  static_assert(!meta::less_t<meta::number<2>, meta::number<1>>::value, "");

  static_assert(!meta::greater_t<meta::number<1>, meta::number<2>>::value, "");
  static_assert(meta::greater_t<meta::number<2>, meta::number<1>>::value, "");

  static_assert(meta::usum_t<0>::value == 0, "");
  static_assert(meta::usum_t<0, 1>::value == 1, "");
  static_assert(meta::usum_t<0, 1, 2>::value == 3, "");
  static_assert(meta::usum_t<0, 1, 2, 3>::value == 6, "");

  using _0 = meta::number<0>;
  using _1 = meta::number<1>;
  using _2 = meta::number<2>;
  using _3 = meta::number<3>;

  static_assert(meta::sum_t<meta::type_list<_0>>::value == 0, "");
  static_assert(meta::sum_t<meta::type_list<_0, _1>>::value == 1, "");
  static_assert(meta::sum_t<meta::type_list<_0, _1, _2>>::value == 3, "");
  static_assert(meta::sum_t<meta::type_list<_0, _1, _2, _3>>::value == 6, "");
}
}
