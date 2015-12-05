#include <archie/utils/meta/as_type_list.h>
#include <type_traits>
#include <catch.hpp>

namespace {
namespace meta = archie::utils::meta;

template <typename... Ts>
struct test {};

TEST_CASE("meta::as_type_list_t") {
  static_assert(std::is_same<meta::as_type_list_t<test<>>, meta::type_list<>>::value, "");
  static_assert(std::is_same<meta::as_type_list_t<test<int>>, meta::type_list<int>>::value, "");
  static_assert(
      std::is_same<meta::as_type_list_t<test<int, char>>, meta::type_list<int, char>>::value, "");
  static_assert(std::is_same<meta::as_type_list_t<test<int, char, double>>,
                             meta::type_list<int, char, double>>::value,
                "");
}
}
