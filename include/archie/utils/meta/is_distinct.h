#ifndef ARCHIE_UTILS_META_IS_DISTINCT_H_INCLUDED
#define ARCHIE_UTILS_META_IS_DISTINCT_H_INCLUDED

#include <type_traits>
#include <archie/utils/meta/type_list.h>

namespace archie {
namespace utils {
  namespace meta {
    namespace detail {
      template <typename Tp, typename... Ts>
      struct is_distinct {
        using type = typename std::conditional<
            type_list<Ts...>::template contains<Tp>::value, std::false_type,
            typename is_distinct<Ts...>::type>::type;
      };

      template <typename Tp>
      struct is_distinct<Tp> {
        using type = std::true_type;
      };
    }

    template <typename... Ts>
    using is_distinct = typename detail::is_distinct<Ts...>::type;
  }
}
}

#endif
