#include <utility>

template <bool B>
using Boolean = std::integral_constant<bool, B>;

namespace detail {

template <typename... Conditions>
struct All;

template <typename Head, typename... Tail>
struct All<Head, Tail...> {
  using type = Boolean<Head::value && detail::All<Tail...>::type::value>;
};

template <typename Head>
struct All<Head> {
  using type = Boolean<Head::value>;
};

template <typename... Conditions>
struct Any;

template <typename Head, typename... Tail>
struct Any<Head, Tail...> {
  using type = Boolean<Head::value || detail::Any<Tail...>::type::value>;
};

template <typename Head>
struct Any<Head> {
  using type = Boolean<Head::value>;
};

template <typename Condition>
struct Not {
  using type = Boolean<!Condition::value>;
};

template <typename... Conditions>
struct None {
  using type = Boolean<
      detail::Not<typename detail::Any<Conditions...>::type>::type::value>;
};

struct enabler {};
}

template <typename... Conditions>
struct All : public detail::All<Conditions...>::type {};

template <typename... Conditions>
struct Any : public detail::Any<Conditions...>::type {};

template <typename Condition>
struct Not : public detail::Not<Condition>::type {};

template <typename... Conditions>
struct None : public detail::None<Conditions...>::type {};

template <typename Condition>
using Requires =
    typename std::enable_if<Condition::value, detail::enabler>::type;

template <typename Condition>
using RequiresAll = Requires<All<Condition>>;

template <typename Condition>
using RequiresAny = Requires<Any<Condition>>;

template <typename Condition>
using RequiresNone = Requires<None<Condition>>;

#include <gtest/gtest.h>
#include <type_traits>

struct requires_test : public ::testing::Test {};

struct test {
  template <typename T, Requires<std::is_unsigned<T>>...>
  int func(T) {
    return 0;
  }

  template <typename T, Requires<std::is_floating_point<T>>...>
  int func(T) {
    return 1;
  }
};

TEST_F(requires_test, canUseBoolean) {
  static_assert(Boolean<true>::value, "");
  static_assert(!Boolean<false>::value, "");
}

TEST_F(requires_test, canUseAll) {
  static_assert(All<std::is_integral<int>>::value, "");
  static_assert(All<std::is_integral<int>, std::is_unsigned<unsigned>>::value,
                "");
  static_assert(!All<std::is_integral<int>, std::is_unsigned<int>>::value, "");
}

TEST_F(requires_test, canUseAny) {
  static_assert(Any<std::is_integral<int>>::value, "");
  static_assert(!Any<std::is_integral<float>>::value, "");
  static_assert(Any<std::is_integral<int>, std::is_unsigned<unsigned>>::value,
                "");
  static_assert(Any<std::is_integral<int>, std::is_unsigned<int>>::value, "");
  static_assert(!Any<std::is_integral<float>, std::is_unsigned<int>>::value,
                "");
}

TEST_F(requires_test, canUseNone) {
  static_assert(!None<std::is_integral<int>>::value, "");
  static_assert(None<std::is_integral<float>>::value, "");
  static_assert(!None<std::is_integral<int>, std::is_unsigned<unsigned>>::value,
                "");
  static_assert(!None<std::is_integral<int>, std::is_unsigned<int>>::value, "");
  static_assert(None<std::is_integral<float>, std::is_unsigned<int>>::value,
                "");
}

TEST_F(requires_test, canUseNot) {
  static_assert(!Not<std::is_integral<int>>::value, "");
  static_assert(Not<std::is_integral<float>>::value, "");
}

TEST_F(requires_test, canUseRequires) {
  unsigned a = 0;
  test t;
  EXPECT_EQ(0, t.func(a));
}
