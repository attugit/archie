#include <archie/utils/meta/returns.h>
#include <archie/utils/meta/transform.h>
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

int main() { return 0; }
