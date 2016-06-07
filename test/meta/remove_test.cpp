#include <archie/meta/remove.hpp>
#include <type_traits>

namespace {
namespace meta = archie::meta;

template <unsigned>
struct utype {
};

template <typename Tp>
using is_0 = std::is_same<utype<0>, Tp>;

using _0 = utype<0>;
using _1 = utype<1>;
static_assert(std::is_same<meta::remove_if_t<is_0, meta::type_list<_0>>, meta::type_list<>>::value,
              "");

static_assert(
    std::is_same<meta::remove_if_t<is_0, meta::type_list<_1>>, meta::type_list<_1>>::value,
    "");

static_assert(
    std::is_same<meta::remove_if_t<is_0, meta::type_list<_1, _0>>, meta::type_list<_1>>::value,
    "");

static_assert(std::is_same<meta::remove_if_t<is_0, meta::type_list<_1, _0, _1>>,
                           meta::type_list<_1, _1>>::value,
              "");

static_assert(std::is_same<meta::remove_if_t<is_0, meta::type_list<_0, _1, _0, _1>>,
                           meta::type_list<_1, _1>>::value,
              "");
}
