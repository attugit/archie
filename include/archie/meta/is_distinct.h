#pragma once

#include <type_traits>
#include <archie/meta/eval.h>
#include <archie/meta/sequence.h>
#include <archie/fused/boolean.h>

namespace archie {
namespace meta {
  namespace detail {
    template <typename...>
    struct is_distinct;

    template <typename Tp, typename... Ts>
    struct is_distinct<Tp, Ts...>
        : eval<std::conditional<sequence<Ts...>::template contains<Tp>::value,
                                decltype(fused::False),
                                is_distinct<Ts...>>> {};

    template <typename Tp>
    struct is_distinct<Tp> : decltype(fused::True) {};

    template <>
    struct is_distinct<> : decltype(fused::True) {};
  }
  template <typename... Ts>
  using is_distinct = eval<detail::is_distinct<Ts...>>;
}
}
