#include <archie/utils/fused/as_tuple.h>
#include <type_traits>

namespace meta = archie::utils::meta;
namespace fused = archie::utils::fused;

template <unsigned>
struct utype {};
using _0 = utype<0>;
using _1 = utype<1>;

namespace {
static_assert(std::is_same<fused::as_tuple<_0>::type, fused::tuple<_0>>::value, "");
static_assert(std::is_same<fused::as_tuple<_0, _1>::type, fused::tuple<_0, _1>>::value, "");
static_assert(std::is_same<fused::as_tuple<_0, _1, _0>::type, fused::tuple<_0, _1, _0>>::value, "");
}
namespace {
static_assert(std::is_same<fused::as_tuple<meta::type_list<_0>>::type, fused::tuple<_0>>::value,
              "");
static_assert(
    std::is_same<fused::as_tuple<meta::type_list<_0, _1>>::type, fused::tuple<_0, _1>>::value,
    "");
static_assert(std::is_same<fused::as_tuple<meta::type_list<_0, _1, _0>>::type,
                           fused::tuple<_0, _1, _0>>::value,
              "");
}

#include <archie/utils/test.h>

void canUseAsTuple() {
  auto x = fused::as_tuple<int, double, char>::make(1, 2.0, '3');

  EXPECT_EQ(1, fused::get<0>(x));
  EXPECT_EQ(2.0, fused::get<1>(x));
  EXPECT_EQ('3', fused::get<2>(x));
}

void canUseAsTupleWithTypeList() {
  auto x = fused::as_tuple<meta::type_list<int, double, char>>::make(1, 2.0, '3');

  EXPECT_EQ(1, fused::get<0>(x));
  EXPECT_EQ(2.0, fused::get<1>(x));
  EXPECT_EQ('3', fused::get<2>(x));
}

int main() {
  canUseAsTuple();
  canUseAsTupleWithTypeList();
  return 0;
}
