#include <archie/meta/index_of.h>
#include <array>
#include <catch.hpp>

namespace {
namespace meta = archie::meta;

static_assert(decltype(meta::type_index<int>(int{}))::value == 0, "");
static_assert(decltype(meta::type_index<int>(char{}, int{}))::value == 1, "");
static_assert(decltype(meta::type_index<int>(char{}, int{}, float{}))::value == 1, "");
static_assert(decltype(meta::type_index<int>(char{}, int{}, float{}, int{}))::value == 1, "");

TEST_CASE("canGetIndexOfSimpleType") {
  char a = '1';
  int b = 2;
  float c = 3;
  REQUIRE(0 == meta::type_index<char>(a, b, c, b));
  REQUIRE(1 == meta::type_index<int>(a, b, c, b));
  REQUIRE(2 == meta::type_index<float>(a, b, c, b));
}

TEST_CASE("canUseIndexOfWithTypeLists") {
  REQUIRE(0 == meta::index_of<char>(meta::type_list<char, int, float>{}));
  REQUIRE(1 == meta::index_of<int>(meta::type_list<char, int, float>{}));
}

TEST_CASE("canUseIndexOfInConsexpr") {
  constexpr auto idx = meta::index_of<char>(meta::type_list<char, int, float>{});
  REQUIRE(0 == idx);
  constexpr auto cnt = meta::index_of<int>(meta::type_list<char, int, float>{});
  std::array<int, cnt> arr;
  REQUIRE(sizeof(int) == sizeof(arr));
}
}
