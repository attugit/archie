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
}

int main() { return 0; }
