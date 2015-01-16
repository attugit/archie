#include <archie/utils/meta/tail.h>
#include <type_traits>

namespace meta = archie::utils::meta;

template <unsigned>
struct utype {};

using _0 = utype<0>;
using _1 = utype<1>;

static_assert(std::is_same<meta::tail_t<0, _0>, meta::type_list<_0>>::value,
              "");
static_assert(std::is_same<meta::tail_t<1, _0>, meta::type_list<>>::value, "");
static_assert(
    std::is_same<meta::tail_t<0, _0, _1>, meta::type_list<_0, _1>>::value, "");
static_assert(std::is_same<meta::tail_t<1, _0, _1>, meta::type_list<_1>>::value,
              "");
static_assert(std::is_same<meta::tail_t<2, _0, _1>, meta::type_list<>>::value,
              "");

int main() { return 0; }
