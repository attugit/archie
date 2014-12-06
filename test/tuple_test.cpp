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

#include <gtest/gtest.h>

namespace {

struct CustomClass {
  static std::size_t instances;
  static std::size_t population;

  CustomClass() : id(++population) {
    ++def_ctor;
    ++instances;
  }
  template <typename... Ts>
  CustomClass(Ts&&...)
      : id(++population) {
    ++ctor;
    ++instances;
  }
  CustomClass(CustomClass const&) : id(++population) {
    ++copy_ctor;
    ++instances;
  }
  CustomClass(CustomClass&& rhs) : id(rhs.id) { ++move_ctor; }
  CustomClass& operator=(CustomClass const&) {
    ++copy_assign;
    return *this;
  }
  CustomClass& operator=(CustomClass&& rhs) {
    id = rhs.id;
    ++move_assign;
    return *this;
  }
  ~CustomClass() { --instances; }
  bool operator==(CustomClass const& rhs) const { return this == &rhs; }
  bool operator<(CustomClass const& rhs) const { return this < &rhs; }

  std::size_t id = 0;
  std::size_t def_ctor = 0;
  std::size_t ctor = 0;
  std::size_t copy_ctor = 0;
  std::size_t move_ctor = 0;
  std::size_t copy_assign = 0;
  std::size_t move_assign = 0;
};

std::size_t CustomClass::instances = 0;
std::size_t CustomClass::population = 0;

struct tuple_test : ::testing::Test {
  void SetUp() {
    ASSERT_EQ(0u, CustomClass::instances);
    ASSERT_EQ(0u, CustomClass::population);
  }
  void TearDown() {
    ASSERT_EQ(0u, CustomClass::instances);
    CustomClass::population = 0;
  }
};

TEST_F(tuple_test, canDefaultConstruct) {
  static_assert(
      std::is_default_constructible<tuple<unsigned, double, char>>::value, "");
  auto t = tuple<unsigned, double, char>();

  // EXPECT_EQ(3u, t.size());
  EXPECT_LE(sizeof(unsigned) + sizeof(double) + sizeof(char), sizeof(t));
}

TEST_F(tuple_test, canConstruct) {
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

TEST_F(tuple_test, canCopyConstruct) {
  static_assert(
      std::is_copy_constructible<tuple<unsigned, double, char>>::value, "");
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
  static_assert(std::is_copy_assignable<tuple<unsigned, double, char>>::value,
                "");
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

TEST_F(tuple_test, canMoveConstruct) {
  static_assert(
      std::is_move_constructible<tuple<unsigned, double, char>>::value, "");
  auto orig = tuple<unsigned, double, char>(1u, 2.0, '3');

  ASSERT_EQ(1u, get<0>(orig));
  ASSERT_EQ(2.0, get<1>(orig));
  ASSERT_EQ('3', get<2>(orig));

  auto copy = tuple<unsigned, double, char>{std::move(orig)};

  EXPECT_EQ(1u, get<0>(copy));
  EXPECT_EQ(2.0, get<1>(copy));
  EXPECT_EQ('3', get<2>(copy));
}

TEST_F(tuple_test, canMoveAssign) {
  static_assert(std::is_move_assignable<tuple<unsigned, double, char>>::value,
                "");
  auto orig = tuple<unsigned, double, char>(1u, 2.0, '3');
  auto copy = tuple<unsigned, double, char>(2u, 4.0, '6');

  ASSERT_EQ(2u, get<0>(copy));
  ASSERT_EQ(4.0, get<1>(copy));
  ASSERT_EQ('6', get<2>(copy));

  copy = std::move(orig);

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
}
