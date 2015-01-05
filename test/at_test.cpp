#include <archie/utils/meta/at.h>
#include <archie/utils/test.h>
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

void canCallAt() {
  auto a = 1u;
  auto b = 2.0;
  auto c = '3';

  auto at_0 = au::meta::at<0>{};
  auto at_1 = au::meta::at<1>{};
  auto at_2 = au::meta::at<2>{};

  auto const& x = at_0(a, b, c);
  auto const& y = at_1(a, b, c);
  auto const& z = at_2(a, b, c);

  EXPECT_EQ(&a, &x);
  EXPECT_EQ(&b, &y);
  EXPECT_EQ(&c, &z);
}

int main() {
  canUseAtMatch();
  canCallAt();
  return 0;
}
