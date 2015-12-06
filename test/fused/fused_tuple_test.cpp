#include <type_traits>
#include <archie/fused/tuple.hpp>
#include <archie/traits.hpp>
#include <memory>
#include <archie/fused/ignore.hpp>
#include <catch.hpp>

namespace {
namespace fused = archie::fused;
namespace traits = archie::traits;
namespace meta = archie::meta;

TEST_CASE("canDefaultConstruct") {
  {
    using tuple_t = fused::tuple<unsigned, double, char>;
    static_assert(std::is_default_constructible<tuple_t>::value, "");
    static_assert(fused::tuple_size(fused::id<tuple_t>) == 3u, "");
    auto t = fused::tuple<unsigned, double, char>();
    static_assert(fused::tuple_size(fused::id<decltype(t)>) == 3u, "");

    REQUIRE(3u == fused::tuple_size(t));
    REQUIRE(sizeof(unsigned) + sizeof(double) + sizeof(char) <= sizeof(t));
  }
  {
    static_assert(
        std::is_default_constructible<fused::tuple<unsigned, std::unique_ptr<double>, char>>::value,
        "");
    auto t = fused::tuple<unsigned, std::unique_ptr<double>, char>();

    REQUIRE(3u == fused::tuple_size(t));
    REQUIRE(sizeof(unsigned) + sizeof(std::unique_ptr<double>) + sizeof(char) <= sizeof(t));
  }
}

TEST_CASE("canDefaultConstructTupleWithUncopyableElement") {
  static_assert(
      std::is_default_constructible<fused::tuple<unsigned, std::unique_ptr<double>, char>>::value,
      "");

  auto t = fused::tuple<unsigned, std::unique_ptr<double>, char>{};

  REQUIRE(3u == fused::tuple_size(t));
  REQUIRE(sizeof(unsigned) + sizeof(std::unique_ptr<double>) + sizeof(char) <= sizeof(t));
}

TEST_CASE("canConstruct") {
  {
    static_assert(
        std::is_constructible<fused::tuple<unsigned, double, char>, unsigned, double, char>::value,
        "");
    auto t = fused::tuple<unsigned, double, char>(1u, 2.0, '3');

    REQUIRE(3u == fused::tuple_size(t));
    REQUIRE(sizeof(unsigned) + sizeof(double) + sizeof(char) <= sizeof(t));
  }
  {
    static_assert(std::is_constructible<fused::tuple<unsigned, std::unique_ptr<double>, char>,
                                        unsigned, std::unique_ptr<double>&&, char>::value,
                  "");
    auto t =
        fused::tuple<unsigned, std::unique_ptr<double>, char>(1u, std::unique_ptr<double>{}, '3');

    REQUIRE(3u == fused::tuple_size(t));
    REQUIRE(sizeof(unsigned) + sizeof(std::unique_ptr<double>) + sizeof(char) <= sizeof(t));
  }
}

TEST_CASE("canConstructTupleWithUncopyableElement") {
  static_assert(std::is_constructible<fused::tuple<unsigned, std::unique_ptr<double>, char>,
                                      unsigned, std::unique_ptr<double>&&, char>::value,
                "");
  auto t =
      fused::tuple<unsigned, std::unique_ptr<double>, char>(1u, std::make_unique<double>(2.0), '3');

  REQUIRE(3u == fused::tuple_size(t));
  REQUIRE(sizeof(unsigned) + sizeof(std::unique_ptr<double>) + sizeof(char) <= sizeof(t));
}

TEST_CASE("makeTupleTakesElementsByValue") {
  unsigned a = 1u;
  double b = 2.0;
  char c = '3';

  auto t = fused::make_tuple(a, b, c);

  REQUIRE(3u == fused::tuple_size(t));

  REQUIRE(a == fused::get<0>(t));
  REQUIRE(b == fused::get<1>(t));
  REQUIRE(c == fused::get<2>(t));

  REQUIRE(&a != &fused::get<0>(t));
  REQUIRE(&b != &fused::get<1>(t));
  REQUIRE(&c != &fused::get<2>(t));
}

TEST_CASE("makeTupleTakesElementsByRValue") {
  auto ptr = std::make_unique<char>('3');
  auto t = fused::make_tuple(1u, std::make_unique<double>(2.0), std::move(ptr));

  REQUIRE(3u == fused::tuple_size(t));

  REQUIRE(2.0 == *fused::get<1>(t));
  REQUIRE('3' == *fused::get<2>(t));
}

TEST_CASE("canUseGetByIdToRead") {
  auto t = fused::make_tuple(1u, 2.0, '3');

  REQUIRE(3u == fused::tuple_size(t));

  REQUIRE(1u == fused::get<0>(t));
  REQUIRE(2.0 == fused::get<1>(t));
  REQUIRE('3' == fused::get<2>(t));

  auto const& x = fused::get<0>(t);
  REQUIRE(1u == x);
  auto const& y = fused::get<0>(t);
  REQUIRE(&x == &y);
}

TEST_CASE("canUseGetByIdToWrite") {
  auto t = fused::make_tuple(1u, std::make_unique<double>(2.0), '3');

  REQUIRE(3u == fused::tuple_size(t));

  auto const& x = fused::get<0>(t);
  REQUIRE(1u == x);
  auto& y = fused::get<0>(t);
  REQUIRE(&x == &y);
  y = 4u;
  REQUIRE(4u == x);
  fused::get<0>(t) = 5u;
  REQUIRE(5u == x);

  REQUIRE(2.0 == *fused::get<1>(t));
  fused::get<1>(t) = std::make_unique<double>(3.0);
  REQUIRE(3.0 == *fused::get<1>(t));
}

TEST_CASE("canUseAtWithTuple") {
  constexpr auto const& at_0 = fused::at<0>;
  constexpr auto const& at_1 = fused::at<1>;
  constexpr auto const& at_2 = fused::at<2>;

  auto t = fused::make_tuple(1u, 2.0, '3');

  REQUIRE(3u == fused::tuple_size(t));

  REQUIRE(1u == at_0(t));
  REQUIRE(2.0 == at_1(t));
  REQUIRE('3' == at_2(t));

  auto const& x = at_0(t);
  REQUIRE(1u == x);
  auto const& y = at_0(t);
  REQUIRE(&x == &y);

  at_0(t) = 4u;
  REQUIRE(4u == x);
}

TEST_CASE("canCopyConstruct") {
  static_assert(
      !std::is_copy_constructible<fused::tuple<unsigned, std::unique_ptr<double>, char>>::value,
      "");
  static_assert(std::is_copy_constructible<fused::tuple<unsigned, double, char>>::value, "");
  auto orig = fused::tuple<unsigned, double, char>(1u, 2.0, '3');
  auto copy = orig;
  auto copy2(orig);

  REQUIRE(3u == fused::tuple_size(orig));
  REQUIRE(3u == fused::tuple_size(copy));
  REQUIRE(3u == fused::tuple_size(copy2));

  REQUIRE(fused::get<0>(orig) == fused::get<0>(copy));
  REQUIRE(fused::get<1>(orig) == fused::get<1>(copy));
  REQUIRE(fused::get<2>(orig) == fused::get<2>(copy));

  REQUIRE(&fused::get<0>(orig) != &fused::get<0>(copy));
  REQUIRE(&fused::get<1>(orig) != &fused::get<1>(copy));
  REQUIRE(&fused::get<2>(orig) != &fused::get<2>(copy));

  REQUIRE(fused::get<0>(orig) == fused::get<0>(copy2));
  REQUIRE(fused::get<1>(orig) == fused::get<1>(copy2));
  REQUIRE(fused::get<2>(orig) == fused::get<2>(copy2));

  REQUIRE(&fused::get<0>(orig) != &fused::get<0>(copy2));
  REQUIRE(&fused::get<1>(orig) != &fused::get<1>(copy2));
  REQUIRE(&fused::get<2>(orig) != &fused::get<2>(copy2));
}

TEST_CASE("canCopyAssign") {
  static_assert(
      std::is_copy_assignable<fused::tuple<unsigned, std::unique_ptr<double>, char>>::value, "");
  static_assert(std::is_copy_assignable<fused::tuple<unsigned, double, char>>::value, "");
  auto orig = fused::tuple<unsigned, double, char>(1u, 2.0, '3');
  auto copy = fused::tuple<unsigned, double, char>(2u, 4.0, '6');

  REQUIRE(3u == fused::tuple_size(orig));
  REQUIRE(3u == fused::tuple_size(copy));

  REQUIRE(2u == fused::get<0>(copy));
  REQUIRE(4.0 == fused::get<1>(copy));
  REQUIRE('6' == fused::get<2>(copy));

  copy = orig;

  REQUIRE(3u == fused::tuple_size(copy));

  REQUIRE(1u == fused::get<0>(copy));
  REQUIRE(2.0 == fused::get<1>(copy));
  REQUIRE('3' == fused::get<2>(copy));
}

TEST_CASE("canMoveConstruct") {
  static_assert(
      std::is_move_constructible<fused::tuple<unsigned, std::unique_ptr<double>, char>>::value, "");
  static_assert(std::is_move_constructible<fused::tuple<unsigned, double, char>>::value, "");
  auto orig = fused::tuple<unsigned, double, char>(1u, 2.0, '3');

  REQUIRE(3u == fused::tuple_size(orig));

  REQUIRE(1u == fused::get<0>(orig));
  REQUIRE(2.0 == fused::get<1>(orig));
  REQUIRE('3' == fused::get<2>(orig));

  auto copy = fused::tuple<unsigned, double, char>{std::move(orig)};

  REQUIRE(3u == fused::tuple_size(copy));

  REQUIRE(1u == fused::get<0>(copy));
  REQUIRE(2.0 == fused::get<1>(copy));
  REQUIRE('3' == fused::get<2>(copy));
}

TEST_CASE("canMoveAssign") {
  static_assert(
      std::is_move_assignable<fused::tuple<unsigned, std::unique_ptr<double>, char>>::value, "");
  static_assert(std::is_move_assignable<fused::tuple<unsigned, double, char>>::value, "");
  auto orig = fused::tuple<unsigned, double, char>(1u, 2.0, '3');
  auto copy = fused::tuple<unsigned, double, char>(2u, 4.0, '6');

  REQUIRE(3u == fused::tuple_size(orig));
  REQUIRE(3u == fused::tuple_size(copy));

  REQUIRE(2u == fused::get<0>(copy));
  REQUIRE(4.0 == fused::get<1>(copy));
  REQUIRE('6' == fused::get<2>(copy));

  copy = std::move(orig);

  REQUIRE(3u == fused::tuple_size(copy));

  REQUIRE(1u == fused::get<0>(copy));
  REQUIRE(2.0 == fused::get<1>(copy));
  REQUIRE('3' == fused::get<2>(copy));
}

TEST_CASE("canCompareEquality") {
  auto t1 = fused::make_tuple(1u, 2.0, '3');
  auto t2 = fused::make_tuple(1u, 2.0, '3');
  auto t3 = fused::make_tuple(0u, 2.0, '3');
  auto t4 = fused::make_tuple(1u, 2.0, '4');

  REQUIRE(t1 == t1);
  REQUIRE(t1 == t2);
  REQUIRE_FALSE(t1 == t3);
  REQUIRE_FALSE(t1 == t4);
}

TEST_CASE("canCompareInequality") {
  auto t1 = fused::make_tuple(1u, 2.0, '3');
  auto t2 = fused::make_tuple(1u, 2.0, '3');
  auto t3 = fused::make_tuple(0u, 2.0, '3');
  auto t4 = fused::make_tuple(1u, 2.0, '4');

  REQUIRE_FALSE(t1 != t1);
  REQUIRE_FALSE(t1 != t2);
  REQUIRE(t1 != t3);
  REQUIRE(t1 != t4);
}

TEST_CASE("canLexicographicalCompareLess") {
  auto t1 = fused::make_tuple(1u, 2.0, '3');
  auto t2 = fused::make_tuple(1u, 2.0, '3');
  auto t3 = fused::make_tuple(0u, 2.0, '3');
  auto t4 = fused::make_tuple(1u, 2.0, '4');
  auto t5 = fused::make_tuple(2u, 2.0, '1');

  REQUIRE_FALSE(t1 < t1);
  REQUIRE_FALSE(t1 < t2);
  REQUIRE_FALSE(t2 < t1);

  REQUIRE_FALSE(t1 < t3);
  REQUIRE(t3 < t1);

  REQUIRE(t1 < t4);
  REQUIRE_FALSE(t4 < t1);

  REQUIRE(t1 < t5);
  REQUIRE_FALSE(t5 < t1);
}

TEST_CASE("canAssignSimilarTuples") {
  auto t = fused::make_tuple(1ul, 2ul, 3ul);
  t = fused::make_tuple(4, 5, 6);

  REQUIRE(4u == fused::get<0>(t));
  REQUIRE(5u == fused::get<1>(t));
  REQUIRE(6u == fused::get<2>(t));
}

TEST_CASE("canStoreValuesInTuple") {
  unsigned a = 1u;
  double b = 2.0;
  char c = '3';

  auto t = fused::tuple<unsigned, double, char>(a, b, c);

  REQUIRE(&a != &fused::get<0>(t));
  REQUIRE(&b != &fused::get<1>(t));
  REQUIRE(&c != &fused::get<2>(t));

  fused::get<0>(t) = 0;
  fused::get<1>(t) = 0;
  fused::get<2>(t) = 0;

  REQUIRE(0u == fused::get<0>(t));
  REQUIRE(0 == fused::get<1>(t));
  REQUIRE(0 == fused::get<2>(t));

  REQUIRE(1u == a);
  REQUIRE(2.0 == b);
  REQUIRE('3' == c);
}

TEST_CASE("canStoreReferencesInTuple") {
  static_assert(
      std::is_constructible<fused::tuple<unsigned, double, char&>, unsigned, double, char&>::value,
      "");
  unsigned a = 1u;
  double b = 2.0;
  char c = '3';

  auto t = fused::tuple<unsigned&, double&, char&>(a, b, c);

  REQUIRE(&a == &fused::get<0>(t));
  REQUIRE(&b == &fused::get<1>(t));
  REQUIRE(&c == &fused::get<2>(t));

  fused::get<0>(t) = 4u;
  fused::get<1>(t) = 5.0;
  fused::get<2>(t) = '6';

  REQUIRE(4u == a);
  REQUIRE(5.0 == b);
  REQUIRE('6' == c);

  t = fused::make_tuple(7u, 8.0, '9');

  REQUIRE(7u == a);
  REQUIRE(8.0 == b);
  REQUIRE('9' == c);
}

TEST_CASE("canTieElements") {
  unsigned a = 1u;
  double b = 2.0;
  char c = '3';

  auto t = fused::tie(a, b, c);

  REQUIRE(1u == fused::get<0>(t));
  REQUIRE(2.0 == fused::get<1>(t));
  REQUIRE('3' == fused::get<2>(t));

  REQUIRE(&a == &fused::get<0>(t));
  REQUIRE(&b == &fused::get<1>(t));
  REQUIRE(&c == &fused::get<2>(t));

  fused::get<0>(t) = 4u;
  fused::get<1>(t) = 5.0;
  fused::get<2>(t) = '6';

  REQUIRE(4u == a);
  REQUIRE(5.0 == b);
  REQUIRE('6' == c);

  t = fused::make_tuple(7u, 8.0, '9');

  REQUIRE(7u == a);
  REQUIRE(8.0 == b);
  REQUIRE('9' == c);
}

TEST_CASE("canTieConstElements") {
  auto a = 1u;
  auto const b = 2.0;
  auto c = '3';

  auto t = fused::tie(a, b, c);
  static_assert(std::is_same<std::remove_reference_t<decltype(t)>,
                             fused::tuple<unsigned&, double const&, char&>>::value,
                "");
  REQUIRE(1u == fused::get<0>(t));
  REQUIRE(2.0 == fused::get<1>(t));
  REQUIRE('3' == fused::get<2>(t));

  REQUIRE(&a == &fused::get<0>(t));
  REQUIRE(&b == &fused::get<1>(t));
  REQUIRE(&c == &fused::get<2>(t));
}

TEST_CASE("canTieWithIgnore") {
  unsigned a = 0;
  char c = 0;

  fused::tie(a, fused::ignore, c) = fused::make_tuple(1u, 2.0, '3');

  REQUIRE(1u == a);
  REQUIRE('3' == c);
}

TEST_CASE("canUseTupleElement") {
  using t = fused::tuple<unsigned, std::unique_ptr<double>, char&>;
  static_assert(std::is_same<fused::tuple_element_t<0, t>, unsigned>::value, "");
  static_assert(std::is_same<fused::tuple_element_t<1, t>, std::unique_ptr<double>>::value, "");
  static_assert(std::is_same<fused::tuple_element_t<2, t>, char&>::value, "");
}

TEST_CASE("canGetElementByType") {
  auto t = fused::make_tuple(1u, 2.0, '3');

  REQUIRE(1u == fused::get<unsigned>(t));
  REQUIRE(2.0 == fused::get<double>(t));
  REQUIRE('3' == fused::get<char>(t));

  fused::get<unsigned>(t) = 4u;
  fused::get<double>(t) = 5.0;
  fused::get<char>(t) = '6';

  REQUIRE(4u == fused::get<unsigned>(t));
  REQUIRE(5.0 == fused::get<double>(t));
  REQUIRE('6' == fused::get<char>(t));
}

TEST_CASE("canExtractElement") {
  constexpr auto const& ex_u = fused::extract<unsigned>;
  constexpr auto const& ex_d = fused::extract<double>;
  constexpr auto const& ex_c = fused::extract<char>;

  auto t = fused::make_tuple(1u, 2.0, '3');

  REQUIRE(1u == ex_u(t));
  REQUIRE(2.0 == ex_d(t));
  REQUIRE('3' == ex_c(t));

  ex_u(t) = 4u;
  ex_d(t) = 5.0;
  ex_c(t) = '6';

  REQUIRE(4u == ex_u(t));
  REQUIRE(5.0 == ex_d(t));
  REQUIRE('6' == ex_c(t));
}

struct conv {
  explicit conv(std::string s) : value(s.size()) {}
  conv(unsigned i) : value(i) {}
  operator std::size_t() const { return value; }
  std::size_t value = 0;
};

#if defined(USE_ARCHIE_TUPLE)
TEST_CASE("canConstructTupleWithExplicitElementCtor") {
  static_assert(traits::is_convertible<unsigned, conv>::value, "");
  static_assert(!traits::is_convertible<std::string&, conv>::value, "");

  std::string a{"abc"};
  auto t1 = fused::tuple<conv>(1u);
  auto t2 = fused::tuple<conv>(a);
  REQUIRE(1u == fused::get<0>(t1));
  REQUIRE(3u == fused::get<0>(t2));
}
#endif

TEST_CASE("canMakeTupleOfTuples") {
  auto t0 = fused::make_tuple(1, 2u);
  auto t1 = fused::tuple<fused::tuple<int, unsigned>>{};
  auto t2 = fused::tuple<fused::tuple<int, unsigned>>{t0};
  auto t3 = fused::tuple<fused::tuple<int, unsigned>, char>{t0, '3'};
  auto t4 =
      fused::tuple<fused::tuple<int, unsigned>, fused::tuple<char>>{t0, fused::make_tuple('3')};

  REQUIRE(2u == fused::tuple_size(t0));
  REQUIRE(1u == fused::tuple_size(t1));
  REQUIRE(1u == fused::tuple_size(t2));
  REQUIRE(2u == fused::tuple_size(t3));
  REQUIRE(2u == fused::tuple_size(t4));
}
}
