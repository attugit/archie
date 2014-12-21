#include <archie/utils/meta/requires.h>
#include <archie/utils/test.h>
#include <type_traits>

using archie::utils::meta::requires;
using archie::utils::meta::boolean;
using archie::utils::meta::all;
using archie::utils::meta::any;
using archie::utils::meta::none;

struct foo {
  template <typename T, requires<std::is_unsigned<T>>...>
  int func(T) {
    return 0;
  }

  template <typename T, requires<std::is_floating_point<T>>...>
  int func(T) {
    return 1;
  }
};

static_assert(boolean<true>::value, "");
static_assert(!boolean<false>::value, "");

static_assert(all<std::is_integral<int>>::value, "");
static_assert(all<std::is_integral<int>, std::is_unsigned<unsigned>>::value,
              "");
static_assert(!all<std::is_integral<int>, std::is_unsigned<int>>::value, "");

static_assert(any<std::is_integral<int>>::value, "");
static_assert(!any<std::is_integral<float>>::value, "");
static_assert(any<std::is_integral<int>, std::is_unsigned<unsigned>>::value,
              "");
static_assert(any<std::is_integral<int>, std::is_unsigned<int>>::value, "");
static_assert(!any<std::is_integral<float>, std::is_unsigned<int>>::value, "");

static_assert(!none<std::is_integral<int>>::value, "");
static_assert(none<std::is_integral<float>>::value, "");
static_assert(!none<std::is_integral<int>, std::is_unsigned<unsigned>>::value,
              "");
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
