#pragma once

#include <type_traits>
#include <archie/utils/meta/eval.h>
#include <archie/utils/meta/sequence.h>

namespace archie {
namespace utils {
  namespace meta {

    namespace detail {
      template <typename...>
      struct is_distinct;

      template <typename Tp, typename... Ts>
      struct is_distinct<Tp, Ts...>
          : eval<std::conditional<sequence<Ts...>::template contains<Tp>::value,
                                  std::false_type, is_distinct<Ts...>>> {};

      template <typename Tp>
      struct is_distinct<Tp> : std::true_type {};

      template <>
      struct is_distinct<> : std::true_type {};
    }
    template <typename... Ts>
    using is_distinct = eval<detail::is_distinct<Ts...>>;
  }
}
}
