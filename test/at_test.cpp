#include <archie/utils/meta/at.h>
#include "test/assert.h"
#include <type_traits>

namespace au = archie::utils;

static_assert(
    std::is_same<int, au::meta::at<1>::apply<char, int, float>::type>::value,
    "");
static_assert(std::is_same<int, au::meta::at_t<1, char, int, float>>::value,
              "");
static_assert(
    std::is_same<int&, au::meta::at_t<1, unsigned, int&, char>>::value, "");

void canUseAtMatch() {
  char a = 0;
  int b = 1;
  float c = 2.0;

  auto& x = au::meta::at<0>::match(a, b, c);
  auto& y = au::meta::at<1>::match(a, b, c);
  auto& z = au::meta::at<2>::match(a, b, c);

  EXPECT_EQ(a, x);
  EXPECT_EQ(b, y);
  EXPECT_EQ(c, z);

  EXPECT_EQ(&a, &x);
  EXPECT_EQ(&b, &y);
  EXPECT_EQ(&c, &z);
}

int main() {
  canUseAtMatch();
  return 0;
}
