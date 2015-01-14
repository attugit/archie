#ifndef ARCHIE_UTILS_META_IS_DISTINCT_H_INCLUDED
#define ARCHIE_UTILS_META_IS_DISTINCT_H_INCLUDED

#include <type_traits>
#include <archie/utils/meta/sequence.h>

namespace archie {
namespace utils {
  namespace meta {

    namespace detail {
      template <typename...>
      struct is_distinct;

      template <typename Tp, typename... Ts>
      struct is_distinct<Tp, Ts...>
          : std::conditional<sequence<Ts...>::template contains<Tp>::value,
                             std::false_type, is_distinct<Ts...>>::type {};

      template <typename Tp>
      struct is_distinct<Tp> : std::true_type {};

      template <>
      struct is_distinct<> : std::true_type {};
    }
    template <typename... Ts>
    using is_distinct = typename detail::is_distinct<Ts...>::type;
  }
}
}

#endif
