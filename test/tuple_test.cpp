#include <type_traits>

#if 1
#include <archie/utils/fused/tuple.h>
namespace fused = archie::utils::fused;
using fused::make_tuple;
using fused::get;
using fused::tuple;
using fused::tuple_size;
namespace test = archie::utils::fused;
#else
#include <tuple>
using std::make_tuple;
using std::get;
using std::tuple;
using std::tuple_size;
namespace test = std;
#endif

#include <gtest/gtest.h>
#include <memory>

namespace {

struct tuple_test : ::testing::Test {};

TEST_F(tuple_test, canDefaultConstruct) {
  static_assert(
      std::is_default_constructible<tuple<unsigned, double, char>>::value, "");
  auto t = tuple<unsigned, double, char>();

  EXPECT_EQ(3u, tuple_size<decltype(t)>::value);
  EXPECT_LE(sizeof(unsigned) + sizeof(double) + sizeof(char), sizeof(t));
}

TEST_F(tuple_test, canDefaultConstructTupleWithUncopyableElement) {
  static_assert(std::is_default_constructible<
                    tuple<unsigned, std::unique_ptr<double>, char>>::value,
                "");

  auto t = tuple<unsigned, std::unique_ptr<double>, char>{};

  EXPECT_EQ(3u, tuple_size<decltype(t)>::value);
  EXPECT_LE(sizeof(unsigned) + sizeof(std::unique_ptr<double>) + sizeof(char),
            sizeof(t));
}

TEST_F(tuple_test, canConstruct) {
  static_assert(std::is_constructible<tuple<unsigned, double, char>, unsigned,
                                      double, char>::value,
                "");
  auto t = tuple<unsigned, double, char>(1u, 2.0, '3');

  EXPECT_EQ(3u, tuple_size<decltype(t)>::value);
  EXPECT_LE(sizeof(unsigned) + sizeof(double) + sizeof(char), sizeof(t));
}

TEST_F(tuple_test, canConstructTupleWithUncopyableElement) {
  static_assert(std::is_constructible <
                    tuple<unsigned, std::unique_ptr<double>, char>,
                unsigned, std::unique_ptr<double>&&, char > ::value, "");
  auto t = tuple<unsigned, std::unique_ptr<double>, char>(
      1u, std::make_unique<double>(2.0), '3');

  EXPECT_EQ(3u, tuple_size<decltype(t)>::value);
  EXPECT_LE(sizeof(unsigned) + sizeof(std::unique_ptr<double>) + sizeof(char),
            sizeof(t));
}

TEST_F(tuple_test, makeTupleTakesElementsByValue) {
  unsigned a = 1u;
  double b = 2.0;
  char c = '3';

  auto t = test::make_tuple(a, b, c);

  ASSERT_EQ(3u, tuple_size<decltype(t)>::value);

  EXPECT_EQ(a, get<0>(t));
  EXPECT_EQ(c, get<2>(t));

  EXPECT_NE(&a, &get<0>(t));
  EXPECT_NE(&b, &get<1>(t));
  EXPECT_NE(&c, &get<2>(t));
}

TEST_F(tuple_test, makeTupleTakesElementsByRValue) {
  auto ptr = std::make_unique<char>('3');
  auto t = test::make_tuple(1u, std::make_unique<double>(2.0), std::move(ptr));

  ASSERT_EQ(3u, tuple_size<decltype(t)>::value);

  EXPECT_EQ(2.0, *get<1>(t));
  EXPECT_EQ('3', *get<2>(t));
}

TEST_F(tuple_test, canUseGetByIdToRead) {
  auto t = test::make_tuple(1u, 2.0, '3');

  ASSERT_EQ(3u, tuple_size<decltype(t)>::value);

  EXPECT_EQ(1u, get<0>(t));
  EXPECT_EQ(2.0, get<1>(t));
  EXPECT_EQ('3', get<2>(t));

  auto const& x = get<0>(t);
  EXPECT_EQ(1u, x);
  auto const& y = get<0>(t);
  EXPECT_EQ(&x, &y);
}

TEST_F(tuple_test, canUseGetByIdToWrite) {
  auto t = test::make_tuple(1u, std::make_unique<double>(2.0), '3');

  ASSERT_EQ(3u, tuple_size<decltype(t)>::value);

  auto const& x = get<0>(t);
  ASSERT_EQ(1u, x);
  auto& y = get<0>(t);
  ASSERT_EQ(&x, &y);
  y = 4u;
  EXPECT_EQ(4u, x);
  get<0>(t) = 5u;
  EXPECT_EQ(5u, x);

  ASSERT_EQ(2.0, *get<1>(t));
  get<1>(t) = std::move(std::make_unique<double>(3.0));
  EXPECT_EQ(3.0, *get<1>(t));
}

TEST_F(tuple_test, canCopyConstruct) {
  static_assert(!std::is_copy_constructible<
                    tuple<unsigned, std::unique_ptr<double>, char>>::value,
                "");
  static_assert(
      std::is_copy_constructible<tuple<unsigned, double, char>>::value, "");
  auto orig = tuple<unsigned, double, char>(1u, 2.0, '3');
  auto copy = orig;

  ASSERT_EQ(3u, tuple_size<decltype(orig)>::value);
  ASSERT_EQ(3u, tuple_size<decltype(copy)>::value);

  EXPECT_EQ(get<0>(orig), get<0>(copy));
  EXPECT_EQ(get<1>(orig), get<1>(copy));
  EXPECT_EQ(get<2>(orig), get<2>(copy));

  EXPECT_NE(&get<0>(orig), &get<0>(copy));
  EXPECT_NE(&get<1>(orig), &get<1>(copy));
  EXPECT_NE(&get<2>(orig), &get<2>(copy));
}

TEST_F(tuple_test, canCopyAssign) {
  static_assert(std::is_copy_assignable<tuple<unsigned, double, char>>::value,
                "");
  auto orig = tuple<unsigned, double, char>(1u, 2.0, '3');
  auto copy = tuple<unsigned, double, char>(2u, 4.0, '6');

  ASSERT_EQ(3u, tuple_size<decltype(orig)>::value);
  ASSERT_EQ(3u, tuple_size<decltype(copy)>::value);

  ASSERT_EQ(2u, get<0>(copy));
  ASSERT_EQ(4.0, get<1>(copy));
  ASSERT_EQ('6', get<2>(copy));

  copy = orig;

  ASSERT_EQ(3u, tuple_size<decltype(copy)>::value);

  EXPECT_EQ(1u, get<0>(copy));
  EXPECT_EQ(2.0, get<1>(copy));
  EXPECT_EQ('3', get<2>(copy));
}

TEST_F(tuple_test, canMoveConstruct) {
  static_assert(
      std::is_move_constructible<tuple<unsigned, double, char>>::value, "");
  auto orig = tuple<unsigned, double, char>(1u, 2.0, '3');

  ASSERT_EQ(3u, tuple_size<decltype(orig)>::value);

  ASSERT_EQ(1u, get<0>(orig));
  ASSERT_EQ(2.0, get<1>(orig));
  ASSERT_EQ('3', get<2>(orig));

  auto copy = tuple<unsigned, double, char>{std::move(orig)};

  ASSERT_EQ(3u, tuple_size<decltype(copy)>::value);

  EXPECT_EQ(1u, get<0>(copy));
  EXPECT_EQ(2.0, get<1>(copy));
  EXPECT_EQ('3', get<2>(copy));
}

TEST_F(tuple_test, canMoveAssign) {
  static_assert(std::is_move_assignable<tuple<unsigned, double, char>>::value,
                "");
  auto orig = tuple<unsigned, double, char>(1u, 2.0, '3');
  auto copy = tuple<unsigned, double, char>(2u, 4.0, '6');

  ASSERT_EQ(3u, tuple_size<decltype(orig)>::value);
  ASSERT_EQ(3u, tuple_size<decltype(copy)>::value);

  ASSERT_EQ(2u, get<0>(copy));
  ASSERT_EQ(4.0, get<1>(copy));
  ASSERT_EQ('6', get<2>(copy));

  copy = std::move(orig);

  ASSERT_EQ(3u, tuple_size<decltype(copy)>::value);

  EXPECT_EQ(1u, get<0>(copy));
  EXPECT_EQ(2.0, get<1>(copy));
  EXPECT_EQ('3', get<2>(copy));
}

TEST_F(tuple_test, canCompareEquality) {
  auto t1 = make_tuple(1u, 2.0, '3');
  auto t2 = make_tuple(1u, 2.0, '3');
  auto t3 = make_tuple(0u, 2.0, '3');
  auto t4 = make_tuple(1u, 2.0, '4');

  EXPECT_TRUE(t1 == t1);
  EXPECT_TRUE(t1 == t2);
  EXPECT_FALSE(t1 == t3);
  EXPECT_FALSE(t1 == t4);
}

TEST_F(tuple_test, canCompareInequality) {
  auto t1 = make_tuple(1u, 2.0, '3');
  auto t2 = make_tuple(1u, 2.0, '3');
  auto t3 = make_tuple(0u, 2.0, '3');
  auto t4 = make_tuple(1u, 2.0, '4');

  EXPECT_FALSE(t1 != t1);
  EXPECT_FALSE(t1 != t2);
  EXPECT_TRUE(t1 != t3);
  EXPECT_TRUE(t1 != t4);
}

TEST_F(tuple_test, canLexicographicalCompareLess) {
  auto t1 = make_tuple(1u, 2.0, '3');
  auto t2 = make_tuple(1u, 2.0, '3');
  auto t3 = make_tuple(0u, 2.0, '3');
  auto t4 = make_tuple(1u, 2.0, '4');
  auto t5 = make_tuple(2u, 2.0, '1');

  EXPECT_FALSE(t1 < t1);
  EXPECT_FALSE(t1 < t2);
  EXPECT_FALSE(t2 < t1);

  EXPECT_FALSE(t1 < t3);
  EXPECT_TRUE(t3 < t1);

  EXPECT_TRUE(t1 < t4);
  EXPECT_FALSE(t4 < t1);

  EXPECT_TRUE(t1 < t5);
  EXPECT_FALSE(t5 < t1);
}

TEST_F(tuple_test, canAssignSimilarTuples) {
  auto t = test::make_tuple(1ul, 2ul, 3ul);
  t = test::make_tuple(4, 5, 6);

  EXPECT_EQ(4u, test::get<0>(t));
  EXPECT_EQ(5u, test::get<1>(t));
  EXPECT_EQ(6u, test::get<2>(t));
}

TEST_F(tuple_test, canStoreReferencesInTuple) {
  unsigned a = 1u;
  double b = 2.0;
  char c = '3';

  auto t = test::tuple<unsigned&, double&, char&>(a, b, c);

  EXPECT_EQ(&a, &test::get<0>(t));
  EXPECT_EQ(&b, &test::get<1>(t));
  EXPECT_EQ(&c, &test::get<2>(t));

  test::get<0>(t) = 4u;
  test::get<1>(t) = 5.0;
  test::get<2>(t) = '6';

  EXPECT_EQ(4u, a);
  EXPECT_EQ(5.0, b);
  EXPECT_EQ('6', c);

  t = test::make_tuple(7u, 8.0, '9');

  EXPECT_EQ(7u, a);
  EXPECT_EQ(8.0, b);
  EXPECT_EQ('9', c);
}
}
