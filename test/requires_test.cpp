#include <archie/meta/requires.h>
#include <archie/test.h>
#include <type_traits>

using archie::meta::requires;
using archie::meta::boolean;
using archie::meta::all;
using archie::meta::any;
using archie::meta::none;

struct foo {
  template <typename T>
  int func(T, requires<std::is_unsigned<T>>* = nullptr) {
    return 0;
  }

  template <typename T>
  int func(T, requires<std::is_floating_point<T>>* = nullptr) {
    return 1;
  }
};

static_assert(boolean<true>::value, "");
static_assert(!boolean<false>::value, "");

static_assert(all<std::is_integral<int>>::value, "");
static_assert(all<std::is_integral<int>, std::is_unsigned<unsigned>>::value, "");
static_assert(!all<std::is_integral<int>, std::is_unsigned<int>>::value, "");

static_assert(any<std::is_integral<int>>::value, "");
static_assert(!any<std::is_integral<float>>::value, "");
static_assert(any<std::is_integral<int>, std::is_unsigned<unsigned>>::value, "");
static_assert(any<std::is_integral<int>, std::is_unsigned<int>>::value, "");
static_assert(!any<std::is_integral<float>, std::is_unsigned<int>>::value, "");

static_assert(!none<std::is_integral<int>>::value, "");
static_assert(none<std::is_integral<float>>::value, "");
static_assert(!none<std::is_integral<int>, std::is_unsigned<unsigned>>::value, "");
static_assert(!none<std::is_integral<int>, std::is_unsigned<int>>::value, "");
static_assert(none<std::is_integral<float>, std::is_unsigned<int>>::value, "");

void canUseRequires() {
  foo t;
  EXPECT_EQ(0, t.func(0u));
  EXPECT_EQ(1, t.func(0.0));
}

int main() {
  canUseRequires();
  return 0;
}
