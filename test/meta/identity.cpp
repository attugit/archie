#include <archie/meta/identity.hpp>
#include <type_traits>

namespace
{
  namespace au = archie;
  static_assert(std::is_same<int, int>::value, "");
  static_assert(std::is_same<au::meta::identity<int>::type, int>::value, "");
  static_assert(std::is_same<au::meta::identity_t<int>, int>::value, "");
}
