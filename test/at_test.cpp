#include <archie/utils/meta/at.h>
#include <archie/utils/test.h>
#include <type_traits>

namespace au = archie::utils;

static_assert(std::is_same<int, au::meta::at_t<1, char, int, float>>::value,
              "");
static_assert(std::is_same<int, au::meta::at_t<1, char, int, float>>::value,
              "");
static_assert(
    std::is_same<int&, au::meta::at_t<1, unsigned, int&, char>>::value, "");
static_assert(
    std::is_same<
        int&,
        au::meta::at_t<1, au::meta::type_list<unsigned, int&, char>>>::value,
    "");

int main() { return 0; }
