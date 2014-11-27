#include <archie/utils/inspect.h>

#include <vector>
#include <utility>

namespace au = archie::utils;

static_assert(au::has_value_type<std::vector<int>>::value, "");
static_assert(!au::has_value_type<std::pair<int, int>>::value, "");
static_assert(!au::has_value_type<int>::value, "");
