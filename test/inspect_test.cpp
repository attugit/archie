#include <archie/utils/inspect.h>

#include <vector>
#include <utility>

namespace au = archie::utils;

static_assert(au::HasValueType<std::vector<int>>::value, "");
static_assert(!au::HasValueType<std::pair<int, int>>::value, "");
