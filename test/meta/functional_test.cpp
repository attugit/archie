#include <archie/meta.hpp>

namespace
{
  namespace meta = archie::meta;

  static_assert(std::is_same<int, meta::if_t<meta::true_t, int, char>>::value, "");
  static_assert(std::is_same<char, meta::if_t<meta::false_t, int, char>>::value, "");

  static_assert(meta::sum<0>::value == 0);
  static_assert(meta::sum<0, 1>::value == 1);
  static_assert(meta::sum<0, 1, 2>::value == 3);
  static_assert(meta::sum<0, 1, 2, 3>::value == 6);
}
