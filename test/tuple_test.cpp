#include <archie/utils/fused/tuple.h>
#include <archie/utils/meta/type_list.h>
#include <type_traits>
namespace {
using archie::utils::fused::tuple;
using archie::utils::meta::type_list;

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

#include <gtest/gtest.h>

namespace {
namespace fused = archie::utils::fused;
using fused::make_tuple;

struct tuple_test : ::testing::Test {};

TEST_F(tuple_test, canCreteTuple) {
  auto t = fused::tuple<unsigned, double, char>();

  EXPECT_EQ(3u, t.size());
}

TEST_F(tuple_test, canCreateTupleWithMakeTuple) {
  auto t1 = make_tuple(1u, 2.0, '3');
  auto t2 = make_tuple(1u, 2.0, 4, '3');
  EXPECT_EQ(3u, t1.size());
  EXPECT_EQ(4u, t2.size());
}

TEST_F(tuple_test, canUseGetByIdToRead) {
  auto t = make_tuple(1u, 2.0, '3');

  EXPECT_EQ(1u, fused::get<0>(t));
  EXPECT_EQ(2.0, fused::get<1>(t));
  EXPECT_EQ('3', fused::get<2>(t));

  auto const& x = fused::get<0>(t);
  EXPECT_EQ(1u, x);
  auto const& y = fused::get<0>(t);
  EXPECT_EQ(&x, &y);
}

TEST_F(tuple_test, canUseGetByIdToWrite) {
  auto t = make_tuple(1u, 2.0, '3');

  auto const& x = fused::get<0>(t);
  ASSERT_EQ(1u, x);
  auto& y = fused::get<0>(t);
  ASSERT_EQ(&x, &y);
  y = 4u;
  EXPECT_EQ(4u, x);
  fused::get<0>(t) = 5u;
  EXPECT_EQ(5u, x);
}

TEST_F(tuple_test, canCopyCreateTuple) {
  auto orig = fused::tuple<unsigned, double, char>(1u, 2.0, '3');
  auto copy = orig;
  ASSERT_EQ(orig.size(), copy.size());

  EXPECT_EQ(fused::get<0>(orig), fused::get<0>(copy));
  EXPECT_EQ(fused::get<1>(orig), fused::get<1>(copy));
  EXPECT_EQ(fused::get<2>(orig), fused::get<2>(copy));

  EXPECT_NE(&fused::get<0>(orig), &fused::get<0>(copy));
  EXPECT_NE(&fused::get<1>(orig), &fused::get<1>(copy));
  EXPECT_NE(&fused::get<2>(orig), &fused::get<2>(copy));
}

TEST_F(tuple_test, canCopyAssign) {
  auto orig = fused::tuple<unsigned, double, char>(1u, 2.0, '3');
  auto copy = fused::tuple<unsigned, double, char>(2u, 4.0, '6');

  ASSERT_EQ(2u, fused::get<0>(copy));
  ASSERT_EQ(4.0, fused::get<1>(copy));
  ASSERT_EQ('6', fused::get<2>(copy));

  copy = orig;

  EXPECT_EQ(1u, fused::get<0>(copy));
  EXPECT_EQ(2.0, fused::get<1>(copy));
  EXPECT_EQ('3', fused::get<2>(copy));
}
}
