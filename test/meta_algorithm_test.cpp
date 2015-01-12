#include <archie/utils/meta/returns.h>
#include <archie/utils/meta/transform.h>
#include <archie/utils/meta/apply.h>
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
}

int main() { return 0; }
