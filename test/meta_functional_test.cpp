#include <archie/utils/meta/functional.h>
#include <archie/utils/meta/number.h>

namespace meta = archie::utils::meta;

namespace {

static_assert(std::is_same<int, meta::if_t<meta::true_t, int, char>>::value,
              "");
static_assert(std::is_same<char, meta::if_t<meta::false_t, int, char>>::value,
              "");
}

namespace {
static_assert(meta::less_t<meta::number<1>, meta::number<2>>::value, "");
static_assert(!meta::less_t<meta::number<2>, meta::number<1>>::value, "");
}

namespace {
static_assert(!meta::greater_t<meta::number<1>, meta::number<2>>::value, "");
static_assert(meta::greater_t<meta::number<2>, meta::number<1>>::value, "");
}

namespace {
static_assert(meta::usum_t<0>::value == 0, "");
static_assert(meta::usum_t<0, 1>::value == 1, "");
static_assert(meta::usum_t<0, 1, 2>::value == 3, "");
static_assert(meta::usum_t<0, 1, 2, 3>::value == 6, "");
}

int main() { return 0; }
