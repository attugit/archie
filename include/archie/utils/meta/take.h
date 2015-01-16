#pragma once

#include <utility>
#include <archie/utils/meta/returns.h>
#include <archie/utils/meta/type_list.h>
#include <archie/utils/meta/at.h>

namespace archie {
namespace utils {
  namespace meta {
    namespace detail {
      template <std::size_t n, typename = std::make_index_sequence<n>>
      struct take;

      template <std::size_t n, std::size_t... other>
      struct take<n, std::index_sequence<other...>> {
      public:
        template <typename... Us>
        using apply = returns<type_list<at_t<other, Us...>...>>;
      };
    }
    template <std::size_t n, typename... Ts>
    struct take : detail::take<n>::template apply<Ts...> {};

    template <std::size_t n, typename... Ts>
    struct take<n, type_list<Ts...>> : take<n, Ts...> {};

    template <std::size_t n, typename... Ts>
    using take_t = eval<take<n, Ts...>>;
  }
}
}
