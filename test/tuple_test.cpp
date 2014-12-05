#include <type_traits>

#if 1
#include <archie/utils/fused/tuple.h>
namespace fused = archie::utils::fused;
using fused::make_tuple;
using fused::get;
using fused::tuple;
#else
#include <tuple>
using std::make_tuple;
using std::get;
using std::tuple;
#endif
/*
using archie::utils::meta::type_list;
namespace {

template <std::size_t N>
using tag = std::integral_constant<std::size_t, N>;

static_assert(sizeof(tuple<>) != 0, "");

static_assert(std::is_same<type_list<>, tuple<>::type_list>::value, "");
static_assert(std::is_same<type_list<tag<0>>, tuple<tag<0>>::type_list>::value,
              "");
static_assert(std::is_same<type_list<tag<0>, tag<1>>,
                           tuple<tag<0>, tag<1>>::type_list>::value,
              "");

static_assert(sizeof(tuple<int, char>) >= (sizeof(int) + sizeof(char)), "");
static_assert(sizeof(tuple<int, char>) <= (2 * sizeof(int)), "");
}
*/
#include <gtest/gtest.h>

namespace {

struct tuple_test : ::testing::Test {};

TEST_F(tuple_test, canCreteTuple) {
  static_assert(
      std::is_default_constructible<tuple<unsigned, double, char>>::value, "");
  auto t = tuple<unsigned, double, char>();

  // EXPECT_EQ(3u, t.size());
  EXPECT_LE(sizeof(unsigned) + sizeof(double) + sizeof(char), sizeof(t));
}

TEST_F(tuple_test, canCreateTupleWithMakeTuple) {
  static_assert(std::is_constructible<tuple<unsigned, double, char>, unsigned,
                                      double, char>::value,
                "");
  auto t = tuple<unsigned, double, char>(1u, 2.0, '3');
  EXPECT_LE(sizeof(unsigned) + sizeof(double) + sizeof(char), sizeof(t));
}

TEST_F(tuple_test, makeTupleTakesElementsByValue) {
  unsigned a = 1u;
  double b = 2.0;
  char c = '3';

  auto t = make_tuple(a, b, c);

  EXPECT_EQ(a, get<0>(t));
  EXPECT_EQ(c, get<2>(t));

  EXPECT_NE(&a, &get<0>(t));
  EXPECT_NE(&b, &get<1>(t));
  EXPECT_NE(&c, &get<2>(t));
}

TEST_F(tuple_test, canUseGetByIdToRead) {
  auto t = make_tuple(1u, 2.0, '3');

  EXPECT_EQ(1u, get<0>(t));
  EXPECT_EQ(2.0, get<1>(t));
  EXPECT_EQ('3', get<2>(t));

  auto const& x = get<0>(t);
  EXPECT_EQ(1u, x);
  auto const& y = get<0>(t);
  EXPECT_EQ(&x, &y);
}

TEST_F(tuple_test, canUseGetByIdToWrite) {
  auto t = make_tuple(1u, 2.0, '3');

  auto const& x = get<0>(t);
  ASSERT_EQ(1u, x);
  auto& y = get<0>(t);
  ASSERT_EQ(&x, &y);
  y = 4u;
  EXPECT_EQ(4u, x);
  get<0>(t) = 5u;
  EXPECT_EQ(5u, x);
}

TEST_F(tuple_test, canCopyCreateTuple) {
  auto orig = tuple<unsigned, double, char>(1u, 2.0, '3');
  auto copy = orig;

  EXPECT_EQ(get<0>(orig), get<0>(copy));
  EXPECT_EQ(get<1>(orig), get<1>(copy));
  EXPECT_EQ(get<2>(orig), get<2>(copy));

  EXPECT_NE(&get<0>(orig), &get<0>(copy));
  EXPECT_NE(&get<1>(orig), &get<1>(copy));
  EXPECT_NE(&get<2>(orig), &get<2>(copy));
}

TEST_F(tuple_test, canCopyAssign) {
  auto orig = tuple<unsigned, double, char>(1u, 2.0, '3');
  auto copy = tuple<unsigned, double, char>(2u, 4.0, '6');

  ASSERT_EQ(2u, get<0>(copy));
  ASSERT_EQ(4.0, get<1>(copy));
  ASSERT_EQ('6', get<2>(copy));

  copy = orig;

  EXPECT_EQ(1u, get<0>(copy));
  EXPECT_EQ(2.0, get<1>(copy));
  EXPECT_EQ('3', get<2>(copy));
}
}
