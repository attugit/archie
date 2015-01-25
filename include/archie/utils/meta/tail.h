#pragma once

#include <utility>
#include <archie/utils/meta/returns.h>
#include <archie/utils/meta/type_list.h>
#include <archie/utils/meta/ignore.h>
#include <archie/utils/meta/number.h>
#include <archie/utils/meta/indexable.h>

namespace archie {
namespace utils {
  namespace meta {
    namespace detail {
      template <std::size_t... other>
      struct tail {
        template <typename... Ts>
        static constexpr decltype(auto) slice(eat<number<other>>..., Ts...) {
          return type_list<Ts...>{};
        }

        template <typename... Ts>
        using apply = returns<decltype(slice(std::declval<Ts>()...))>;
      };
    }

    template <std::size_t n, typename... Ts>
    struct tail : indexable_t<detail::tail, n>::template apply<Ts...> {};

    template <std::size_t n, typename... Ts>
    struct tail<n, type_list<Ts...>> : tail<n, Ts...> {};

    template <std::size_t n, typename... Ts>
    using tail_t = eval<tail<n, Ts...>>;
  }
}
}
