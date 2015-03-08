#include <archie/utils/meta/index_of.h>
#include <archie/utils/test.h>

namespace meta = archie::utils::meta;

static_assert(decltype(meta::type_index<int>(int{}))::value == 0, "");
static_assert(decltype(meta::type_index<int>(char{}, int{}))::value == 1, "");
static_assert(decltype(meta::type_index<int>(char{}, int{}, float{}))::value == 1, "");
static_assert(decltype(meta::type_index<int>(char{}, int{}, float{}, int{}))::value == 1, "");

void canGetIndexOfSimpleType() {
  char a = '1';
  int b = 2;
  float c = 3;
  EXPECT_EQ(0, meta::type_index<char>(a, b, c, b));
  EXPECT_EQ(1, meta::type_index<int>(a, b, c, b));
  EXPECT_EQ(2, meta::type_index<float>(a, b, c, b));
}

void canUseIndexOfWithTypeLists() {
  EXPECT_EQ(0, meta::index_of<char>(meta::type_list<char, int, float>{}));
  EXPECT_EQ(1, meta::index_of<int>(meta::type_list<char, int, float>{}));
}

#include <array>
void canUseIndexOfInConsexpr() {
  constexpr auto idx = meta::index_of<char>(meta::type_list<char, int, float>{});
  EXPECT_EQ(0, idx);
  constexpr auto cnt = meta::index_of<int>(meta::type_list<char, int, float>{});
  std::array<int, cnt> arr;
  EXPECT_EQ(sizeof(int), sizeof(arr));
}

int main() {
  canGetIndexOfSimpleType();
  canUseIndexOfWithTypeLists();
  canUseIndexOfInConsexpr();
  return 0;
}
