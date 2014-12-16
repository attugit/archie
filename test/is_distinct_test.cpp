#include <archie/utils/meta/is_distinct.h>

namespace {
namespace meta = archie::utils::meta;

static_assert(meta::is_distinct<int>::value, "");
static_assert(meta::is_distinct<int, char, float>::value, "");
static_assert(!meta::is_distinct<int, char, int>::value, "");
static_assert(!meta::is_distinct<int, int, int>::value, "");
static_assert(!meta::is_distinct<char, int, int>::value, "");
}
