#include <archie/meta/identity.h>
#include <type_traits>
#include <catch.hpp>

namespace {
namespace au = archie;
TEST_CASE("meta::identity") {
  static_assert(std::is_same<int, int>::value, "");
  static_assert(std::is_same<au::meta::identity<int>::type, int>::value, "");
  static_assert(std::is_same<au::meta::identity_t<int>, int>::value, "");
}
}
