#include <archie/index_of.hpp>
#include <array>
#include <gtest/gtest.h>

namespace {
using namespace archie::fused;

static_assert(type_index<int>(int{}) == 0);
static_assert(type_index<int>(char{}, int{}) == 1);
static_assert(type_index<int>(char{}, int{}, float{}) == 1);
static_assert(type_index<int>(char{}, int{}, float{}, int{}) == 1);

TEST(index_of, canGetIndexOfSimpleType)
{
  char a = '1';
  int b = 2;
  float c = 3;
  EXPECT_EQ(0, type_index<char>(a, b, c, b));
  EXPECT_EQ(1, type_index<int>(a, b, c, b));
  EXPECT_EQ(2, type_index<float>(a, b, c, b));
}

TEST(index_of, canUseIndexOfWithTypeLists)
{
  EXPECT_EQ(0, index_of<char>(type_list<char, int, float>));
  EXPECT_EQ(1, index_of<int>(type_list<char, int, float>));
  static_assert(0 == index_of<char>(type_list<char, int, float, int, double>));
  static_assert(1 == index_of<int>(type_list<char, int, float, int, double>));
  static_assert(2 == index_of<float>(type_list<char, int, float, int, double>));
  static_assert(4 == index_of<double>(type_list<char, int, float, int, double>));
  static_assert(5 == index_of<void>(type_list<char, int, float, int, double>));
}

TEST(index_of, canUseIndexOfInConsexpr)
{
  constexpr auto idx = index_of<char>(type_list<char, int, float>);
  EXPECT_EQ(0, idx);
  constexpr auto cnt = index_of<int>(type_list<char, int, float>);
  std::array<int, cnt> arr;
  EXPECT_EQ(sizeof(int), sizeof(arr));
}
}
