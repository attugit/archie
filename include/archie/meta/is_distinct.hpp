#pragma once

#include <type_traits>
#include <archie/meta/eval.hpp>
#include <archie/meta/logic.hpp>
#include <archie/fused/boolean.hpp>

namespace archie::meta
{
  template <typename T, typename... U>
  using contains = any<std::is_same<T, U>...>;

  namespace detail
  {
    template <typename...>
    struct is_distinct : decltype(fused::True) {
    };

    template <typename T, typename... U>
    struct is_distinct<T, U...> : eval<std::conditional<contains<T, U...>::value,
                                                        decltype(fused::False),
                                                        eval<is_distinct<U...>>>> {
    };
  }

  template <typename... Ts>
  using is_distinct = eval<detail::is_distinct<Ts...>>;
}
