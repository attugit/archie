#include <type_traits>

#include <archie/utils/fused/tuple.h>
namespace test = archie::utils::fused;
namespace traits = archie::utils::traits;
namespace meta = archie::utils::meta;

#include <archie/utils/test.h>
#include <memory>

void canDefaultConstruct() {
  {
    static_assert(std::is_default_constructible<
                      test::tuple<unsigned, double, char>>::value,
                  "");
    static_assert(test::tuple_size_n(
                      test::type_tag<test::tuple<unsigned, double, char>>{}) ==
                      3u,
                  "");
    auto t = test::tuple<unsigned, double, char>();
    static_assert(test::tuple_size_n(test::type_tag<decltype(t)>{}) == 3u, "");

    EXPECT_EQ(3u, test::tuple_size_n(t));
    EXPECT_LE(sizeof(unsigned) + sizeof(double) + sizeof(char), sizeof(t));
  }
  {
    static_assert(
        std::is_default_constructible<
            test::tuple<unsigned, std::unique_ptr<double>, char>>::value,
        "");
    auto t = test::tuple<unsigned, std::unique_ptr<double>, char>();

    EXPECT_EQ(3u, test::tuple_size_n(t));
    EXPECT_LE(sizeof(unsigned) + sizeof(std::unique_ptr<double>) + sizeof(char),
              sizeof(t));
  }
}

void canDefaultConstructTupleWithUncopyableElement() {
  static_assert(
      std::is_default_constructible<
          test::tuple<unsigned, std::unique_ptr<double>, char>>::value,
      "");

  auto t = test::tuple<unsigned, std::unique_ptr<double>, char>{};

  EXPECT_EQ(3u, test::tuple_size_n(t));
  EXPECT_LE(sizeof(unsigned) + sizeof(std::unique_ptr<double>) + sizeof(char),
            sizeof(t));
}

void canConstruct() {
  {
    static_assert(std::is_constructible<test::tuple<unsigned, double, char>,
                                        unsigned, double, char>::value,
                  "");
    auto t = test::tuple<unsigned, double, char>(1u, 2.0, '3');

    EXPECT_EQ(3u, test::tuple_size_n(t));
    EXPECT_LE(sizeof(unsigned) + sizeof(double) + sizeof(char), sizeof(t));
  }
  {
    static_assert(std::is_constructible <
                      test::tuple<unsigned, std::unique_ptr<double>, char>,
                  unsigned, std::unique_ptr<double>&&, char > ::value, "");
    auto t = test::tuple<unsigned, std::unique_ptr<double>, char>(
        1u, std::unique_ptr<double>{}, '3');

    EXPECT_EQ(3u, test::tuple_size_n(t));
    EXPECT_LE(sizeof(unsigned) + sizeof(std::unique_ptr<double>) + sizeof(char),
              sizeof(t));
  }
}

void canConstructTupleWithUncopyableElement() {
  static_assert(std::is_constructible <
                    test::tuple<unsigned, std::unique_ptr<double>, char>,
                unsigned, std::unique_ptr<double>&&, char > ::value, "");
  auto t = test::tuple<unsigned, std::unique_ptr<double>, char>(
      1u, std::make_unique<double>(2.0), '3');

  EXPECT_EQ(3u, test::tuple_size_n(t));
  EXPECT_LE(sizeof(unsigned) + sizeof(std::unique_ptr<double>) + sizeof(char),
            sizeof(t));
}

void makeTupleTakesElementsByValue() {
  unsigned a = 1u;
  double b = 2.0;
  char c = '3';

  auto t = test::make_tuple(a, b, c);

  ASSERT_EQ(3u, test::tuple_size_n(t));

  EXPECT_EQ(a, test::get<0>(t));
  EXPECT_EQ(b, test::get<1>(t));
  EXPECT_EQ(c, test::get<2>(t));

  EXPECT_NE(&a, &test::get<0>(t));
  EXPECT_NE(&b, &test::get<1>(t));
  EXPECT_NE(&c, &test::get<2>(t));
}

void makeTupleTakesElementsByRValue() {
  auto ptr = std::make_unique<char>('3');
  auto t = test::make_tuple(1u, std::make_unique<double>(2.0), std::move(ptr));

  ASSERT_EQ(3u, test::tuple_size_n(t));

  EXPECT_EQ(2.0, *test::get<1>(t));
  EXPECT_EQ('3', *test::get<2>(t));
}

void canUseGetByIdToRead() {
  auto t = test::make_tuple(1u, 2.0, '3');

  ASSERT_EQ(3u, test::tuple_size_n(t));

  EXPECT_EQ(1u, test::get<0>(t));
  EXPECT_EQ(2.0, test::get<1>(t));
  EXPECT_EQ('3', test::get<2>(t));

  auto const& x = test::get<0>(t);
  EXPECT_EQ(1u, x);
  auto const& y = test::get<0>(t);
  EXPECT_EQ(&x, &y);
}

void canUseGetByIdToWrite() {
  auto t = test::make_tuple(1u, std::make_unique<double>(2.0), '3');

  ASSERT_EQ(3u, test::tuple_size_n(t));

  auto const& x = test::get<0>(t);
  ASSERT_EQ(1u, x);
  auto& y = test::get<0>(t);
  ASSERT_EQ(&x, &y);
  y = 4u;
  EXPECT_EQ(4u, x);
  test::get<0>(t) = 5u;
  EXPECT_EQ(5u, x);

  ASSERT_EQ(2.0, *test::get<1>(t));
  test::get<1>(t) = std::move(std::make_unique<double>(3.0));
  EXPECT_EQ(3.0, *test::get<1>(t));
}

void canCopyConstruct() {
  static_assert(
      !std::is_copy_constructible<
          test::tuple<unsigned, std::unique_ptr<double>, char>>::value,
      "");
  static_assert(
      std::is_copy_constructible<test::tuple<unsigned, double, char>>::value,
      "");
  auto orig = test::tuple<unsigned, double, char>(1u, 2.0, '3');
  auto copy = orig;
  auto copy2(orig);

  ASSERT_EQ(3u, test::tuple_size_n(orig));
  ASSERT_EQ(3u, test::tuple_size_n(copy));
  ASSERT_EQ(3u, test::tuple_size_n(copy2));

  EXPECT_EQ(test::get<0>(orig), test::get<0>(copy));
  EXPECT_EQ(test::get<1>(orig), test::get<1>(copy));
  EXPECT_EQ(test::get<2>(orig), test::get<2>(copy));

  EXPECT_NE(&test::get<0>(orig), &test::get<0>(copy));
  EXPECT_NE(&test::get<1>(orig), &test::get<1>(copy));
  EXPECT_NE(&test::get<2>(orig), &test::get<2>(copy));

  EXPECT_EQ(test::get<0>(orig), test::get<0>(copy2));
  EXPECT_EQ(test::get<1>(orig), test::get<1>(copy2));
  EXPECT_EQ(test::get<2>(orig), test::get<2>(copy2));

  EXPECT_NE(&test::get<0>(orig), &test::get<0>(copy2));
  EXPECT_NE(&test::get<1>(orig), &test::get<1>(copy2));
  EXPECT_NE(&test::get<2>(orig), &test::get<2>(copy2));
}

void canCopyAssign() {
  static_assert(
      std::is_copy_assignable<
          test::tuple<unsigned, std::unique_ptr<double>, char>>::value,
      "");
  static_assert(
      std::is_copy_assignable<test::tuple<unsigned, double, char>>::value, "");
  auto orig = test::tuple<unsigned, double, char>(1u, 2.0, '3');
  auto copy = test::tuple<unsigned, double, char>(2u, 4.0, '6');

  ASSERT_EQ(3u, test::tuple_size_n(orig));
  ASSERT_EQ(3u, test::tuple_size_n(copy));

  ASSERT_EQ(2u, test::get<0>(copy));
  ASSERT_EQ(4.0, test::get<1>(copy));
  ASSERT_EQ('6', test::get<2>(copy));

  copy = orig;

  ASSERT_EQ(3u, test::tuple_size_n(copy));

  EXPECT_EQ(1u, test::get<0>(copy));
  EXPECT_EQ(2.0, test::get<1>(copy));
  EXPECT_EQ('3', test::get<2>(copy));
}

void canMoveConstruct() {
  static_assert(
      std::is_move_constructible<
          test::tuple<unsigned, std::unique_ptr<double>, char>>::value,
      "");
  static_assert(
      std::is_move_constructible<test::tuple<unsigned, double, char>>::value,
      "");
  auto orig = test::tuple<unsigned, double, char>(1u, 2.0, '3');

  ASSERT_EQ(3u, test::tuple_size_n(orig));

  ASSERT_EQ(1u, test::get<0>(orig));
  ASSERT_EQ(2.0, test::get<1>(orig));
  ASSERT_EQ('3', test::get<2>(orig));

  auto copy = test::tuple<unsigned, double, char>{std::move(orig)};

  ASSERT_EQ(3u, test::tuple_size_n(copy));

  EXPECT_EQ(1u, test::get<0>(copy));
  EXPECT_EQ(2.0, test::get<1>(copy));
  EXPECT_EQ('3', test::get<2>(copy));
}

void canMoveAssign() {
  static_assert(
      std::is_move_assignable<
          test::tuple<unsigned, std::unique_ptr<double>, char>>::value,
      "");
  static_assert(
      std::is_move_assignable<test::tuple<unsigned, double, char>>::value, "");
  auto orig = test::tuple<unsigned, double, char>(1u, 2.0, '3');
  auto copy = test::tuple<unsigned, double, char>(2u, 4.0, '6');

  ASSERT_EQ(3u, test::tuple_size_n(orig));
  ASSERT_EQ(3u, test::tuple_size_n(copy));

  ASSERT_EQ(2u, test::get<0>(copy));
  ASSERT_EQ(4.0, test::get<1>(copy));
  ASSERT_EQ('6', test::get<2>(copy));

  copy = std::move(orig);

  ASSERT_EQ(3u, test::tuple_size_n(copy));

  EXPECT_EQ(1u, test::get<0>(copy));
  EXPECT_EQ(2.0, test::get<1>(copy));
  EXPECT_EQ('3', test::get<2>(copy));
}

void canCompareEquality() {
  auto t1 = test::make_tuple(1u, 2.0, '3');
  auto t2 = test::make_tuple(1u, 2.0, '3');
  auto t3 = test::make_tuple(0u, 2.0, '3');
  auto t4 = test::make_tuple(1u, 2.0, '4');

  EXPECT_TRUE(t1 == t1);
  EXPECT_TRUE(t1 == t2);
  EXPECT_FALSE(t1 == t3);
  EXPECT_FALSE(t1 == t4);
}

void canCompareInequality() {
  auto t1 = test::make_tuple(1u, 2.0, '3');
  auto t2 = test::make_tuple(1u, 2.0, '3');
  auto t3 = test::make_tuple(0u, 2.0, '3');
  auto t4 = test::make_tuple(1u, 2.0, '4');

  EXPECT_FALSE(t1 != t1);
  EXPECT_FALSE(t1 != t2);
  EXPECT_TRUE(t1 != t3);
  EXPECT_TRUE(t1 != t4);
}

void canLexicographicalCompareLess() {
  auto t1 = test::make_tuple(1u, 2.0, '3');
  auto t2 = test::make_tuple(1u, 2.0, '3');
  auto t3 = test::make_tuple(0u, 2.0, '3');
  auto t4 = test::make_tuple(1u, 2.0, '4');
  auto t5 = test::make_tuple(2u, 2.0, '1');

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

void canAssignSimilarTuples() {
  auto t = test::make_tuple(1ul, 2ul, 3ul);
  t = test::make_tuple(4, 5, 6);

  EXPECT_EQ(4u, test::get<0>(t));
  EXPECT_EQ(5u, test::get<1>(t));
  EXPECT_EQ(6u, test::get<2>(t));
}

void canStoreValuesInTuple() {
  unsigned a = 1u;
  double b = 2.0;
  char c = '3';

  auto t = test::tuple<unsigned, double, char>(a, b, c);

  EXPECT_NE(&a, &test::get<0>(t));
  EXPECT_NE(&b, &test::get<1>(t));
  EXPECT_NE(&c, &test::get<2>(t));

  test::get<0>(t) = 0;
  test::get<1>(t) = 0;
  test::get<2>(t) = 0;

  ASSERT_EQ(0u, test::get<0>(t));
  ASSERT_EQ(0, test::get<1>(t));
  ASSERT_EQ(0, test::get<2>(t));

  EXPECT_EQ(1u, a);
  EXPECT_EQ(2.0, b);
  EXPECT_EQ('3', c);
}

void canStoreReferencesInTuple() {
  static_assert(std::is_constructible<test::tuple<unsigned, double, char&>,
                                      unsigned, double, char&>::value,
                "");
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

void canTieElements() {
  unsigned a = 1u;
  double b = 2.0;
  char c = '3';

  auto t = test::tie(a, b, c);

  ASSERT_EQ(1u, test::get<0>(t));
  ASSERT_EQ(2.0, test::get<1>(t));
  ASSERT_EQ('3', test::get<2>(t));

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

void canTieConstElements() {
  auto a = 1u;
  auto const b = 2.0;
  auto c = '3';

  auto t = test::tie(a, b, c);
  static_assert(
      std::is_same<std::remove_reference_t<decltype(t)>,
                   test::tuple<unsigned&, double const&, char&>>::value,
      "");
  ASSERT_EQ(1u, test::get<0>(t));
  ASSERT_EQ(2.0, test::get<1>(t));
  ASSERT_EQ('3', test::get<2>(t));

  EXPECT_EQ(&a, &test::get<0>(t));
  EXPECT_EQ(&b, &test::get<1>(t));
  EXPECT_EQ(&c, &test::get<2>(t));
}

#include <archie/utils/fused/ignore.h>

void canTieWithIgnore() {
  unsigned a = 0;
  char c = 0;

  test::tie(a, test::ignore, c) = test::make_tuple(1u, 2.0, '3');

  EXPECT_EQ(1u, a);
  EXPECT_EQ('3', c);
}

void canUseTupleElement() {
  using t = test::tuple<unsigned, std::unique_ptr<double>, char&>;
  static_assert(std::is_same<test::tuple_element_t<0, t>, unsigned>::value, "");
  static_assert(
      std::is_same<test::tuple_element_t<1, t>, std::unique_ptr<double>>::value,
      "");
  static_assert(std::is_same<test::tuple_element_t<2, t>, char&>::value, "");
}

void canGetElementByType() {
  auto t = test::make_tuple(1u, 2.0, '3');

  EXPECT_EQ(1u, test::get<unsigned>(t));
  EXPECT_EQ(2.0, test::get<double>(t));
  EXPECT_EQ('3', test::get<char>(t));

  test::get<unsigned>(t) = 4u;
  test::get<double>(t) = 5.0;
  test::get<char>(t) = '6';

  EXPECT_EQ(4u, test::get<unsigned>(t));
  EXPECT_EQ(5.0, test::get<double>(t));
  EXPECT_EQ('6', test::get<char>(t));
}

struct conv {
  explicit conv(std::string s) : value(s.size()) {}
  conv(unsigned i) : value(i) {}
  operator unsigned() const { return value; }
  unsigned value = 0;
};

#if defined(USE_ARCHIE_TUPLE)
void canConstructTupleWithExplicitElementCtor() {
  static_assert(traits::is_convertible<unsigned, conv>::value, "");
  static_assert(!traits::is_convertible<std::string&, conv>::value, "");

  std::string a{"abc"};
  auto t1 = test::tuple<conv>(1u);
  auto t2 = test::tuple<conv>(a);
  EXPECT_EQ(1u, test::get<0>(t1));
  EXPECT_EQ(3u, test::get<0>(t2));
}
#endif

void canMakeTupleOfTuples() {
  auto t0 = test::make_tuple(1, 2u);
  auto t1 = test::tuple<test::tuple<int, unsigned>>{};
  auto t2 = test::tuple<test::tuple<int, unsigned>>{t0};
  auto t3 = test::tuple<test::tuple<int, unsigned>, char>{t0, '3'};
  auto t4 = test::tuple<test::tuple<int, unsigned>, test::tuple<char>>{
      t0, test::make_tuple('3')};

  ASSERT_EQ(2u, test::tuple_size_n(t0));
  ASSERT_EQ(1u, test::tuple_size_n(t1));
  ASSERT_EQ(1u, test::tuple_size_n(t2));
  ASSERT_EQ(2u, test::tuple_size_n(t3));
  ASSERT_EQ(2u, test::tuple_size_n(t4));
}

int main() {
  canDefaultConstruct();
  canDefaultConstructTupleWithUncopyableElement();
  canConstruct();
  canConstructTupleWithUncopyableElement();
  makeTupleTakesElementsByValue();
  makeTupleTakesElementsByRValue();
  canUseGetByIdToRead();
  canUseGetByIdToWrite();
  canCopyConstruct();
  canCopyAssign();
  canMoveConstruct();
  canMoveAssign();
  canCompareEquality();
  canCompareInequality();
  canLexicographicalCompareLess();
  canAssignSimilarTuples();
  canStoreValuesInTuple();
  canStoreReferencesInTuple();
  canTieElements();
  canTieConstElements();
  canTieWithIgnore();
  canUseTupleElement();
  canGetElementByType();
#if defined(USE_ARCHIE_TUPLE)
  canConstructTupleWithExplicitElementCtor();
#endif
  canMakeTupleOfTuples();

  return 0;
}
