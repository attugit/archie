#include "archie/utils/meta/requires.h"
#include <gtest/gtest.h>
#include <type_traits>

using archie::utils::meta::requires;
using archie::utils::meta::Boolean;
using archie::utils::meta::all;
using archie::utils::meta::any;
using archie::utils::meta::none;

struct requires_test : public ::testing::Test {};

struct test {
  /*  template <typename T, Requires<std::is_unsigned<T>>...>
    int func(T) {
      return 0;
    }

    template <typename T, Requires<std::is_floating_point<T>>...>
    int func(T) {
      return 1;
    }*/
};

TEST_F(requires_test, canUseBoolean) {
  static_assert(Boolean<true>::value, "");
  static_assert(!Boolean<false>::value, "");
}

TEST_F(requires_test, canUseAll) {
  static_assert(all<std::is_integral<int>>::value, "");
  static_assert(all<std::is_integral<int>, std::is_unsigned<unsigned>>::value,
                "");
  static_assert(!all<std::is_integral<int>, std::is_unsigned<int>>::value, "");
}

TEST_F(requires_test, canUseAny) {
  static_assert(any<std::is_integral<int>>::value, "");
  static_assert(!any<std::is_integral<float>>::value, "");
  static_assert(any<std::is_integral<int>, std::is_unsigned<unsigned>>::value,
                "");
  static_assert(any<std::is_integral<int>, std::is_unsigned<int>>::value, "");
  static_assert(!any<std::is_integral<float>, std::is_unsigned<int>>::value,
                "");
}

TEST_F(requires_test, canUseNone) {
  static_assert(!none<std::is_integral<int>>::value, "");
  static_assert(none<std::is_integral<float>>::value, "");
  static_assert(!none<std::is_integral<int>, std::is_unsigned<unsigned>>::value,
                "");
  static_assert(!none<std::is_integral<int>, std::is_unsigned<int>>::value, "");
  static_assert(none<std::is_integral<float>, std::is_unsigned<int>>::value,
                "");
}

TEST_F(requires_test, canUseRequires) {
  //  unsigned a = 0;
  //  test t;
  // EXPECT_EQ(0, t.func(a));
}
