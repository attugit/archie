#include <archie/utils/meta/returns.h>
#include <archie/utils/meta/transform.h>
#include <archie/utils/meta/apply.h>
#include <archie/utils/meta/append.h>
#include <archie/utils/meta/identity.h>
#include <archie/utils/meta/find.h>
#include <type_traits>

namespace meta = archie::utils::meta;

namespace {

template <typename Tp>
struct wrapper {
  using type = Tp;
};

template <typename Tp>
using make_wrapper = meta::returns<wrapper<Tp>>;

static_assert(
    std::is_same<meta::transform_t<make_wrapper>, meta::type_list<>>::value,
    "");

static_assert(std::is_same<meta::transform_t<make_wrapper, int>,
                           meta::type_list<wrapper<int>>>::value,
              "");

static_assert(
    std::is_same<
        meta::transform_t<make_wrapper, int, char, int>,
        meta::type_list<wrapper<int>, wrapper<char>, wrapper<int>>>::value,
    "");

static_assert(
    std::is_same<
        meta::transform_t<make_wrapper, meta::type_list<int, char, int>>,
        meta::type_list<wrapper<int>, wrapper<char>, wrapper<int>>>::value,
    "");
}

namespace {

template <typename... Ts>
struct vsize {
  using type = std::integral_constant<unsigned, sizeof...(Ts)>;
};

static_assert(meta::apply_t<vsize>::value == 0, "");
static_assert(meta::apply_t<vsize, int>::value == 1, "");
static_assert(meta::apply_t<vsize, int, char>::value == 2, "");
static_assert(meta::apply_t<vsize, int, char, int>::value == 3, "");

static_assert(meta::apply_t<vsize, meta::type_list<>>::value == 0, "");
static_assert(meta::apply_t<vsize, meta::type_list<int>>::value == 1, "");
static_assert(meta::apply_t<vsize, meta::type_list<int, char>>::value == 2, "");
static_assert(meta::apply_t<vsize, meta::type_list<int, char, int>>::value == 3,
              "");

static_assert(std::is_same<meta::append_t<meta::type_list<>, int>,
                           meta::type_list<int>>::value,
              "");
static_assert(std::is_same<meta::append_t<meta::type_list<char>, int>,
                           meta::type_list<char, int>>::value,
              "");
static_assert(
    std::is_same<
        meta::append_t<meta::type_list<char>, meta::type_list<int, float>>,
        meta::type_list<char, int, float>>::value,
    "");

static_assert(std::is_same<meta::identity<int>::type, int>::value, "");
static_assert(std::is_same<meta::identity_t<int>, int>::value, "");

static_assert(meta::find_t<int>::value == 0, "");
static_assert(meta::find_t<int, int>::value == 0, "");
static_assert(meta::find_t<int, int, char>::value == 0, "");
static_assert(meta::find_t<int, char, int>::value == 1, "");
static_assert(meta::find_t<int, char, int, int>::value == 1, "");

static_assert(meta::find_t<int, meta::type_list<>>::value == 0, "");
static_assert(meta::find_t<int, meta::type_list<int>>::value == 0, "");
static_assert(meta::find_t<int, meta::type_list<int, char>>::value == 0, "");
static_assert(meta::find_t<int, meta::type_list<char, int>>::value == 1, "");
static_assert(meta::find_t<int, meta::type_list<char, int, int>>::value == 1,
              "");

template <unsigned I>
struct utype {};

template <typename Tp>
using is_0 = std::is_same<utype<0>, Tp>;

template <typename Tp>
using is_1 = std::is_same<utype<1>, Tp>;

static_assert(meta::find_if_t<is_0, utype<0>>::value == 0, "");
static_assert(meta::find_if_t<is_1, utype<0>>::value == 1, "");
static_assert(meta::find_if_t<is_1, utype<0>, utype<1>>::value == 1, "");
static_assert(meta::find_if_t<is_1, utype<0>, utype<1>, utype<1>>::value == 1,
              "");

static_assert(meta::find_if_t<is_0, meta::type_list<utype<0>>>::value == 0, "");
static_assert(meta::find_if_t<is_1, meta::type_list<utype<0>>>::value == 1, "");
static_assert(
    meta::find_if_t<is_1, meta::type_list<utype<0>, utype<1>>>::value == 1, "");
static_assert(meta::find_if_t<
                  is_1, meta::type_list<utype<0>, utype<1>, utype<1>>>::value ==
                  1,
              "");
}

namespace {
using _0 = utype<0>;
using _1 = utype<1>;
static_assert(std::is_same<meta::append_if_t<is_0, meta::type_list<_0, _1>, _1>,
                           meta::type_list<_0, _1>>::value,
              "");
static_assert(std::is_same<meta::append_if_t<is_0, meta::type_list<_0, _1>, _0>,
                           meta::type_list<_0, _1, _0>>::value,
              "");
}

int main() { return 0; }
