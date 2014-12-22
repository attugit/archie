#include <archie/utils/meta/index_of.h>
#include <archie/utils/test.h>

namespace meta = archie::utils::meta;

static_assert(decltype(meta::index_of<int>(int{}))::value == 0, "");
static_assert(decltype(meta::index_of<int>(char{}, int{}))::value == 1, "");
static_assert(decltype(meta::index_of<int>(char{}, int{}, float{}))::value == 1,
              "");
static_assert(decltype(meta::index_of<int>(char{}, int{}, float{},
                                           int{}))::value == 1,
              "");

void canGetIndexOfSimpleType() {
  char a = '1';
  int b = 2;
  float c = 3;
  EXPECT_EQ(0, meta::index_of<char>(a, b, c, b));
  EXPECT_EQ(1, meta::index_of<int>(a, b, c, b));
  EXPECT_EQ(2, meta::index_of<float>(a, b, c, b));
}

int main() {
  canGetIndexOfSimpleType();
  return 0;
}
