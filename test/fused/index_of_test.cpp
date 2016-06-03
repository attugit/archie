#include <archie/index_of.hpp>
#include <array>
#include <catch.hpp>

namespace
{
  using namespace archie::fused;

  static_assert(type_index<int>(int{}) == 0);
  static_assert(type_index<int>(char{}, int{}) == 1);
  static_assert(type_index<int>(char{}, int{}, float{}) == 1);
  static_assert(type_index<int>(char{}, int{}, float{}, int{}) == 1);

  TEST_CASE("canGetIndexOfSimpleType")
  {
    char a = '1';
    int b = 2;
    float c = 3;
    REQUIRE(0 == type_index<char>(a, b, c, b));
    REQUIRE(1 == type_index<int>(a, b, c, b));
    REQUIRE(2 == type_index<float>(a, b, c, b));
  }

  TEST_CASE("canUseIndexOfWithTypeLists")
  {
    REQUIRE(0 == index_of<char>(type_list<char, int, float>));
    REQUIRE(1 == index_of<int>(type_list<char, int, float>));
    static_assert(0 == index_of<char>(type_list<char, int, float, int, double>));
    static_assert(1 == index_of<int>(type_list<char, int, float, int, double>));
    static_assert(2 == index_of<float>(type_list<char, int, float, int, double>));
    static_assert(4 == index_of<double>(type_list<char, int, float, int, double>));
    static_assert(5 == index_of<void>(type_list<char, int, float, int, double>));
  }

  TEST_CASE("canUseIndexOfInConsexpr")
  {
    constexpr auto idx = index_of<char>(type_list<char, int, float>);
    REQUIRE(0 == idx);
    constexpr auto cnt = index_of<int>(type_list<char, int, float>);
    std::array<int, cnt> arr;
    REQUIRE(sizeof(int) == sizeof(arr));
  }
}
