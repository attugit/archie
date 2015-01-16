#include <archie/utils/meta/functional.h>
#include <archie/utils/meta/boolean.h>

namespace meta = archie::utils::meta;

namespace {

static_assert(std::is_same<int, meta::if_t<meta::true_t, int, char>>::value,
              "");
static_assert(std::is_same<char, meta::if_t<meta::false_t, int, char>>::value,
              "");
}

int main() { return 0; }
