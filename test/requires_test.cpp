#include "archie/utils/requires.h"
#include <gtest/gtest.h>
#include <type_traits>

using archie::utils::Requires;
using archie::utils::Boolean;
using archie::utils::All;
using archie::utils::Any;
using archie::utils::None;
using archie::utils::Not;

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
