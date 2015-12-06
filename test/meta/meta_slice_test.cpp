#include <archie/meta/slice.h>
#include <type_traits>
#include <catch.hpp>

namespace {
namespace meta = archie::meta;

template <unsigned>
struct utype {};

using _0 = utype<0>;
using _1 = utype<1>;

TEST_CASE("meta::slice") {
  static_assert(std::is_same<meta::slice_t<0, 1, _0>, meta::type_list<_0>>::value, "");
  static_assert(std::is_same<meta::slice_t<0, 0, _0>, meta::type_list<>>::value, "");
  static_assert(std::is_same<meta::slice_t<1, 1, _0>, meta::type_list<>>::value, "");
  static_assert(std::is_same<meta::slice_t<0, 1, _0, _1>, meta::type_list<_0>>::value, "");
  static_assert(std::is_same<meta::slice_t<1, 2, _0, _1>, meta::type_list<_1>>::value, "");
  static_assert(std::is_same<meta::slice_t<0, 2, _0, _1>, meta::type_list<_0, _1>>::value, "");

  static_assert(std::is_same<meta::slice_t<0, 1, meta::type_list<_0>>, meta::type_list<_0>>::value,
                "");
  static_assert(std::is_same<meta::slice_t<0, 0, meta::type_list<_0>>, meta::type_list<>>::value,
                "");
  static_assert(std::is_same<meta::slice_t<1, 1, meta::type_list<_0>>, meta::type_list<>>::value,
                "");
  static_assert(
      std::is_same<meta::slice_t<0, 1, meta::type_list<_0, _1>>, meta::type_list<_0>>::value, "");
  static_assert(
      std::is_same<meta::slice_t<1, 2, meta::type_list<_0, _1>>, meta::type_list<_1>>::value, "");
  static_assert(
      std::is_same<meta::slice_t<0, 2, meta::type_list<_0, _1>>, meta::type_list<_0, _1>>::value,
      "");
}
}
