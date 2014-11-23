#include <archie/utils/identity.h>
#include <type_traits>

namespace {
namespace au = archie::utils;

static_assert(std::is_same<int, int>::value, "");
static_assert(std::is_same<au::meta::identity<int>::type, int>::value, "");
static_assert(std::is_same<au::meta::identity_t<int>, int>::value, "");
}
