#include <archie/utils/meta/listed.h>
#include <type_traits>

namespace meta = archie::utils::meta;

template <typename... Ts>
struct test {};

static_assert(std::is_same<meta::listed_t<test, meta::type_list<>>, test<>>::value, "");
static_assert(std::is_same<meta::listed_t<test, meta::type_list<int>>, test<int>>::value, "");
static_assert(
    std::is_same<meta::listed_t<test, meta::type_list<int, char>>, test<int, char>>::value,
    "");
static_assert(std::is_same<meta::listed_t<test, meta::type_list<int, char, double>>,
                           test<int, char, double>>::value,
              "");

int main() { return 0; }
