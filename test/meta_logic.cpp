#include <archie/utils/meta/logic.h>

namespace meta = archie::utils::meta;

static_assert(meta::conjunction_t<>::value, "");
static_assert(meta::conjunction_t<true>::value, "");
static_assert(meta::conjunction_t<true, true>::value, "");
static_assert(!meta::conjunction_t<false>::value, "");
static_assert(!meta::conjunction_t<false, true>::value, "");
static_assert(!meta::conjunction_t<true, false>::value, "");
static_assert(!meta::conjunction_t<false, false>::value, "");

static_assert(!meta::disjunction_t<>::value, "");
static_assert(!meta::disjunction_t<false>::value, "");
static_assert(!meta::disjunction_t<false, false>::value, "");
static_assert(meta::disjunction_t<true>::value, "");
static_assert(meta::disjunction_t<true, false>::value, "");
static_assert(meta::disjunction_t<false, true>::value, "");

int main() { return 0; }
