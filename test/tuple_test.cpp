#include <type_traits>

#include <archie/utils/fused/tuple.h>
#include <archie/utils/traits.h>
namespace fused = archie::utils::fused;
namespace traits = archie::utils::traits;
namespace meta = archie::utils::meta;

#include <archie/utils/test.h>
#include <memory>

void canDefaultConstruct() {
  {
    static_assert(std::is_default_constructible<
                      fused::tuple<unsigned, double, char>>::value,
                  "");
    static_assert(
        fused::tuple_size(
            fused::type_tag<fused::tuple<unsigned, double, char>>{}) == 3u,
        "");
    auto t = fused::tuple<unsigned, double, char>();
    static_assert(fused::tuple_size(fused::type_tag<decltype(t)>{}) == 3u, "");

    EXPECT_EQ(3u, fused::tuple_size(t));
    EXPECT_LE(sizeof(unsigned) + sizeof(double) + sizeof(char), sizeof(t));
  }
  {
    static_assert(
        std::is_default_constructible<
            fused::tuple<unsigned, std::unique_ptr<double>, char>>::value,
        "");
    auto t = fused::tuple<unsigned, std::unique_ptr<double>, char>();

    EXPECT_EQ(3u, fused::tuple_size(t));
    EXPECT_LE(sizeof(unsigned) + sizeof(std::unique_ptr<double>) + sizeof(char),
              sizeof(t));
  }
}

void canDefaultConstructTupleWithUncopyableElement() {
  static_assert(
      std::is_default_constructible<
          fused::tuple<unsigned, std::unique_ptr<double>, char>>::value,
      "");

  auto t = fused::tuple<unsigned, std::unique_ptr<double>, char>{};

  EXPECT_EQ(3u, fused::tuple_size(t));
  EXPECT_LE(sizeof(unsigned) + sizeof(std::unique_ptr<double>) + sizeof(char),
            sizeof(t));
}

void canConstruct() {
  {
    static_assert(std::is_constructible<fused::tuple<unsigned, double, char>,
                                        unsigned, double, char>::value,
                  "");
    auto t = fused::tuple<unsigned, double, char>(1u, 2.0, '3');

    EXPECT_EQ(3u, fused::tuple_size(t));
    EXPECT_LE(sizeof(unsigned) + sizeof(double) + sizeof(char), sizeof(t));
  }
  {
    static_assert(std::is_constructible <
                      fused::tuple<unsigned, std::unique_ptr<double>, char>,
                  unsigned, std::unique_ptr<double>&&, char > ::value, "");
    auto t = fused::tuple<unsigned, std::unique_ptr<double>, char>(
        1u, std::unique_ptr<double>{}, '3');

    EXPECT_EQ(3u, fused::tuple_size(t));
    EXPECT_LE(sizeof(unsigned) + sizeof(std::unique_ptr<double>) + sizeof(char),
              sizeof(t));
  }
}

void canConstructTupleWithUncopyableElement() {
  static_assert(std::is_constructible <
                    fused::tuple<unsigned, std::unique_ptr<double>, char>,
                unsigned, std::unique_ptr<double>&&, char > ::value, "");
  auto t = fused::tuple<unsigned, std::unique_ptr<double>, char>(
      1u, std::make_unique<double>(2.0), '3');

  EXPECT_EQ(3u, fused::tuple_size(t));
  EXPECT_LE(sizeof(unsigned) + sizeof(std::unique_ptr<double>) + sizeof(char),
            sizeof(t));
}

void makeTupleTakesElementsByValue() {
  unsigned a = 1u;
  double b = 2.0;
  char c = '3';

  auto t = fused::make_tuple(a, b, c);

  ASSERT_EQ(3u, fused::tuple_size(t));

  EXPECT_EQ(a, fused::get<0>(t));
  EXPECT_EQ(b, fused::get<1>(t));
  EXPECT_EQ(c, fused::get<2>(t));

  EXPECT_NE(&a, &fused::get<0>(t));
  EXPECT_NE(&b, &fused::get<1>(t));
  EXPECT_NE(&c, &fused::get<2>(t));
}

void makeTupleTakesElementsByRValue() {
  auto ptr = std::make_unique<char>('3');
  auto t = fused::make_tuple(1u, std::make_unique<double>(2.0), std::move(ptr));

  ASSERT_EQ(3u, fused::tuple_size(t));

  EXPECT_EQ(2.0, *fused::get<1>(t));
  EXPECT_EQ('3', *fused::get<2>(t));
}

void canUseGetByIdToRead() {
  auto t = fused::make_tuple(1u, 2.0, '3');

  ASSERT_EQ(3u, fused::tuple_size(t));

  EXPECT_EQ(1u, fused::get<0>(t));
  EXPECT_EQ(2.0, fused::get<1>(t));
  EXPECT_EQ('3', fused::get<2>(t));

  auto const& x = fused::get<0>(t);
  EXPECT_EQ(1u, x);
  auto const& y = fused::get<0>(t);
  EXPECT_EQ(&x, &y);
}

void canUseGetByIdToWrite() {
  auto t = fused::make_tuple(1u, std::make_unique<double>(2.0), '3');

  ASSERT_EQ(3u, fused::tuple_size(t));

  auto const& x = fused::get<0>(t);
  ASSERT_EQ(1u, x);
  auto& y = fused::get<0>(t);
  ASSERT_EQ(&x, &y);
  y = 4u;
  EXPECT_EQ(4u, x);
  fused::get<0>(t) = 5u;
  EXPECT_EQ(5u, x);

  ASSERT_EQ(2.0, *fused::get<1>(t));
  fused::get<1>(t) = std::move(std::make_unique<double>(3.0));
  EXPECT_EQ(3.0, *fused::get<1>(t));
}

void canCopyConstruct() {
  static_assert(
      !std::is_copy_constructible<
          fused::tuple<unsigned, std::unique_ptr<double>, char>>::value,
      "");
  static_assert(
      std::is_copy_constructible<fused::tuple<unsigned, double, char>>::value,
      "");
  auto orig = fused::tuple<unsigned, double, char>(1u, 2.0, '3');
  auto copy = orig;
  auto copy2(orig);

  ASSERT_EQ(3u, fused::tuple_size(orig));
  ASSERT_EQ(3u, fused::tuple_size(copy));
  ASSERT_EQ(3u, fused::tuple_size(copy2));

  EXPECT_EQ(fused::get<0>(orig), fused::get<0>(copy));
  EXPECT_EQ(fused::get<1>(orig), fused::get<1>(copy));
  EXPECT_EQ(fused::get<2>(orig), fused::get<2>(copy));

  EXPECT_NE(&fused::get<0>(orig), &fused::get<0>(copy));
  EXPECT_NE(&fused::get<1>(orig), &fused::get<1>(copy));
  EXPECT_NE(&fused::get<2>(orig), &fused::get<2>(copy));

  EXPECT_EQ(fused::get<0>(orig), fused::get<0>(copy2));
  EXPECT_EQ(fused::get<1>(orig), fused::get<1>(copy2));
  EXPECT_EQ(fused::get<2>(orig), fused::get<2>(copy2));

  EXPECT_NE(&fused::get<0>(orig), &fused::get<0>(copy2));
  EXPECT_NE(&fused::get<1>(orig), &fused::get<1>(copy2));
  EXPECT_NE(&fused::get<2>(orig), &fused::get<2>(copy2));
}

void canCopyAssign() {
  static_assert(
      std::is_copy_assignable<
          fused::tuple<unsigned, std::unique_ptr<double>, char>>::value,
      "");
  static_assert(
      std::is_copy_assignable<fused::tuple<unsigned, double, char>>::value, "");
  auto orig = fused::tuple<unsigned, double, char>(1u, 2.0, '3');
  auto copy = fused::tuple<unsigned, double, char>(2u, 4.0, '6');

  ASSERT_EQ(3u, fused::tuple_size(orig));
  ASSERT_EQ(3u, fused::tuple_size(copy));

  ASSERT_EQ(2u, fused::get<0>(copy));
  ASSERT_EQ(4.0, fused::get<1>(copy));
  ASSERT_EQ('6', fused::get<2>(copy));

  copy = orig;

  ASSERT_EQ(3u, fused::tuple_size(copy));

  EXPECT_EQ(1u, fused::get<0>(copy));
  EXPECT_EQ(2.0, fused::get<1>(copy));
  EXPECT_EQ('3', fused::get<2>(copy));
}

void canMoveConstruct() {
  static_assert(
      std::is_move_constructible<
          fused::tuple<unsigned, std::unique_ptr<double>, char>>::value,
      "");
  static_assert(
      std::is_move_constructible<fused::tuple<unsigned, double, char>>::value,
      "");
  auto orig = fused::tuple<unsigned, double, char>(1u, 2.0, '3');

  ASSERT_EQ(3u, fused::tuple_size(orig));

  ASSERT_EQ(1u, fused::get<0>(orig));
  ASSERT_EQ(2.0, fused::get<1>(orig));
  ASSERT_EQ('3', fused::get<2>(orig));

  auto copy = fused::tuple<unsigned, double, char>{std::move(orig)};

  ASSERT_EQ(3u, fused::tuple_size(copy));

  EXPECT_EQ(1u, fused::get<0>(copy));
  EXPECT_EQ(2.0, fused::get<1>(copy));
  EXPECT_EQ('3', fused::get<2>(copy));
}

void canMoveAssign() {
  static_assert(
      std::is_move_assignable<
          fused::tuple<unsigned, std::unique_ptr<double>, char>>::value,
      "");
  static_assert(
      std::is_move_assignable<fused::tuple<unsigned, double, char>>::value, "");
  auto orig = fused::tuple<unsigned, double, char>(1u, 2.0, '3');
  auto copy = fused::tuple<unsigned, double, char>(2u, 4.0, '6');

  ASSERT_EQ(3u, fused::tuple_size(orig));
  ASSERT_EQ(3u, fused::tuple_size(copy));

  ASSERT_EQ(2u, fused::get<0>(copy));
  ASSERT_EQ(4.0, fused::get<1>(copy));
  ASSERT_EQ('6', fused::get<2>(copy));

  copy = std::move(orig);

  ASSERT_EQ(3u, fused::tuple_size(copy));

  EXPECT_EQ(1u, fused::get<0>(copy));
  EXPECT_EQ(2.0, fused::get<1>(copy));
  EXPECT_EQ('3', fused::get<2>(copy));
}

void canCompareEquality() {
  auto t1 = fused::make_tuple(1u, 2.0, '3');
  auto t2 = fused::make_tuple(1u, 2.0, '3');
  auto t3 = fused::make_tuple(0u, 2.0, '3');
  auto t4 = fused::make_tuple(1u, 2.0, '4');

  EXPECT_TRUE(t1 == t1);
  EXPECT_TRUE(t1 == t2);
  EXPECT_FALSE(t1 == t3);
  EXPECT_FALSE(t1 == t4);
}

void canCompareInequality() {
  auto t1 = fused::make_tuple(1u, 2.0, '3');
  auto t2 = fused::make_tuple(1u, 2.0, '3');
  auto t3 = fused::make_tuple(0u, 2.0, '3');
  auto t4 = fused::make_tuple(1u, 2.0, '4');

  EXPECT_FALSE(t1 != t1);
  EXPECT_FALSE(t1 != t2);
  EXPECT_TRUE(t1 != t3);
  EXPECT_TRUE(t1 != t4);
}

void canLexicographicalCompareLess() {
  auto t1 = fused::make_tuple(1u, 2.0, '3');
  auto t2 = fused::make_tuple(1u, 2.0, '3');
  auto t3 = fused::make_tuple(0u, 2.0, '3');
  auto t4 = fused::make_tuple(1u, 2.0, '4');
  auto t5 = fused::make_tuple(2u, 2.0, '1');

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
  auto t = fused::make_tuple(1ul, 2ul, 3ul);
  t = fused::make_tuple(4, 5, 6);

  EXPECT_EQ(4u, fused::get<0>(t));
  EXPECT_EQ(5u, fused::get<1>(t));
  EXPECT_EQ(6u, fused::get<2>(t));
}

void canStoreValuesInTuple() {
  unsigned a = 1u;
  double b = 2.0;
  char c = '3';

  auto t = fused::tuple<unsigned, double, char>(a, b, c);

  EXPECT_NE(&a, &fused::get<0>(t));
  EXPECT_NE(&b, &fused::get<1>(t));
  EXPECT_NE(&c, &fused::get<2>(t));

  fused::get<0>(t) = 0;
  fused::get<1>(t) = 0;
  fused::get<2>(t) = 0;

  ASSERT_EQ(0u, fused::get<0>(t));
  ASSERT_EQ(0, fused::get<1>(t));
  ASSERT_EQ(0, fused::get<2>(t));

  EXPECT_EQ(1u, a);
  EXPECT_EQ(2.0, b);
  EXPECT_EQ('3', c);
}

void canStoreReferencesInTuple() {
  static_assert(std::is_constructible<fused::tuple<unsigned, double, char&>,
                                      unsigned, double, char&>::value,
                "");
  unsigned a = 1u;
  double b = 2.0;
  char c = '3';

  auto t = fused::tuple<unsigned&, double&, char&>(a, b, c);

  EXPECT_EQ(&a, &fused::get<0>(t));
  EXPECT_EQ(&b, &fused::get<1>(t));
  EXPECT_EQ(&c, &fused::get<2>(t));

  fused::get<0>(t) = 4u;
  fused::get<1>(t) = 5.0;
  fused::get<2>(t) = '6';

  EXPECT_EQ(4u, a);
  EXPECT_EQ(5.0, b);
  EXPECT_EQ('6', c);

  t = fused::make_tuple(7u, 8.0, '9');

  EXPECT_EQ(7u, a);
  EXPECT_EQ(8.0, b);
  EXPECT_EQ('9', c);
}

void canTieElements() {
  unsigned a = 1u;
  double b = 2.0;
  char c = '3';

  auto t = fused::tie(a, b, c);

  ASSERT_EQ(1u, fused::get<0>(t));
  ASSERT_EQ(2.0, fused::get<1>(t));
  ASSERT_EQ('3', fused::get<2>(t));

  EXPECT_EQ(&a, &fused::get<0>(t));
  EXPECT_EQ(&b, &fused::get<1>(t));
  EXPECT_EQ(&c, &fused::get<2>(t));

  fused::get<0>(t) = 4u;
  fused::get<1>(t) = 5.0;
  fused::get<2>(t) = '6';

  EXPECT_EQ(4u, a);
  EXPECT_EQ(5.0, b);
  EXPECT_EQ('6', c);

  t = fused::make_tuple(7u, 8.0, '9');

  EXPECT_EQ(7u, a);
  EXPECT_EQ(8.0, b);
  EXPECT_EQ('9', c);
}

void canTieConstElements() {
  auto a = 1u;
  auto const b = 2.0;
  auto c = '3';

  auto t = fused::tie(a, b, c);
  static_assert(
      std::is_same<std::remove_reference_t<decltype(t)>,
                   fused::tuple<unsigned&, double const&, char&>>::value,
      "");
  ASSERT_EQ(1u, fused::get<0>(t));
  ASSERT_EQ(2.0, fused::get<1>(t));
  ASSERT_EQ('3', fused::get<2>(t));

  EXPECT_EQ(&a, &fused::get<0>(t));
  EXPECT_EQ(&b, &fused::get<1>(t));
  EXPECT_EQ(&c, &fused::get<2>(t));
}

#include <archie/utils/fused/ignore.h>

void canTieWithIgnore() {
  unsigned a = 0;
  char c = 0;

  fused::tie(a, fused::ignore, c) = fused::make_tuple(1u, 2.0, '3');

  EXPECT_EQ(1u, a);
  EXPECT_EQ('3', c);
}

void canUseTupleElement() {
  using t = fused::tuple<unsigned, std::unique_ptr<double>, char&>;
  static_assert(std::is_same<fused::tuple_element_t<0, t>, unsigned>::value,
                "");
  static_assert(std::is_same<fused::tuple_element_t<1, t>,
                             std::unique_ptr<double>>::value,
                "");
  static_assert(std::is_same<fused::tuple_element_t<2, t>, char&>::value, "");
}

void canGetElementByType() {
  auto t = fused::make_tuple(1u, 2.0, '3');

  EXPECT_EQ(1u, fused::get<unsigned>(t));
  EXPECT_EQ(2.0, fused::get<double>(t));
  EXPECT_EQ('3', fused::get<char>(t));

  fused::get<unsigned>(t) = 4u;
  fused::get<double>(t) = 5.0;
  fused::get<char>(t) = '6';

  EXPECT_EQ(4u, fused::get<unsigned>(t));
  EXPECT_EQ(5.0, fused::get<double>(t));
  EXPECT_EQ('6', fused::get<char>(t));
}

void canExtractElement() {
#if defined(HAS_VARIABLE_TEMPLATES)
  constexpr auto const& ex_u = fused::extract<unsigned>;
  constexpr auto const& ex_d = fused::extract<double>;
  constexpr auto const& ex_c = fused::extract<char>;
#else
  constexpr auto const& ex_u = fused::extract<unsigned>::value;
  constexpr auto const& ex_d = fused::extract<double>::value;
  constexpr auto const& ex_c = fused::extract<char>::value;
#endif

  auto t = fused::make_tuple(1u, 2.0, '3');

  EXPECT_EQ(1u, ex_u(t));
  EXPECT_EQ(2.0, ex_d(t));
  EXPECT_EQ('3', ex_c(t));

  ex_u(t) = 4u;
  ex_d(t) = 5.0;
  ex_c(t) = '6';

  EXPECT_EQ(4u, ex_u(t));
  EXPECT_EQ(5.0, ex_d(t));
  EXPECT_EQ('6', ex_c(t));
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
  auto t1 = fused::tuple<conv>(1u);
  auto t2 = fused::tuple<conv>(a);
  EXPECT_EQ(1u, fused::get<0>(t1));
  EXPECT_EQ(3u, fused::get<0>(t2));
}
#endif

void canMakeTupleOfTuples() {
  auto t0 = fused::make_tuple(1, 2u);
  auto t1 = fused::tuple<fused::tuple<int, unsigned>>{};
  auto t2 = fused::tuple<fused::tuple<int, unsigned>>{t0};
  auto t3 = fused::tuple<fused::tuple<int, unsigned>, char>{t0, '3'};
  auto t4 = fused::tuple<fused::tuple<int, unsigned>, fused::tuple<char>>{
      t0, fused::make_tuple('3')};

  ASSERT_EQ(2u, fused::tuple_size(t0));
  ASSERT_EQ(1u, fused::tuple_size(t1));
  ASSERT_EQ(1u, fused::tuple_size(t2));
  ASSERT_EQ(2u, fused::tuple_size(t3));
  ASSERT_EQ(2u, fused::tuple_size(t4));
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
  canExtractElement();
#if defined(USE_ARCHIE_TUPLE)
  canConstructTupleWithExplicitElementCtor();
#endif
  canMakeTupleOfTuples();

  return 0;
}
