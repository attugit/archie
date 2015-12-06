#include <archie/meta/is_distinct.h>
#include <catch.hpp>

namespace {
namespace meta = archie::meta;
TEST_CASE("meta::is_distinct") {
  static_assert(meta::is_distinct<int>::value, "");
  static_assert(meta::is_distinct<int, char, float>::value, "");
  static_assert(!meta::is_distinct<int, char, int>::value, "");
  static_assert(!meta::is_distinct<int, int, int>::value, "");
  static_assert(!meta::is_distinct<char, int, int>::value, "");
}
}
