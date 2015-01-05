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

void canCallAt() {
  auto a = 1u;
  auto b = 2.0;
  auto c = '3';

  auto _0 = au::meta::at<0>{};
  auto _1 = au::meta::at<1>{};
  auto _2 = au::meta::at<2>{};

  auto const& x = _0(a, b, c);
  auto const& y = _1(a, b, c);
  auto const& z = _2(a, b, c);

  EXPECT_EQ(&a, &x);
  EXPECT_EQ(&b, &y);
  EXPECT_EQ(&c, &z);
}

int main() {
  canCallAt();
  return 0;
}
