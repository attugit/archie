#include <archie/traits/is_fused_tuple.h>
#include <archie/fused/tuple.h>

namespace meta = archie::meta;
namespace fused = archie::fused;
namespace traits = archie::traits;

static_assert(traits::is_fused_tuple<fused::tuple<int>>::value, "");
static_assert(!traits::is_fused_tuple<int>::value, "");
static_assert(!traits::is_fused_tuple<meta::type_list<int>>::value, "");
static_assert(!traits::is_fused_tuple<meta::type_list<fused::tuple<int>>>::value, "");
static_assert(traits::is_fused_tuple<fused::tuple<meta::type_list<int>>>::value, "");

int main() { return 0; }
