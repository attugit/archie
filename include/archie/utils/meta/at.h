#pragma once

#include <utility>
#include <archie/utils/meta/ignore.h>
#include <archie/utils/meta/returns.h>
#include <archie/utils/meta/number.h>
#include <archie/utils/meta/type_list.h>
#include <archie/utils/meta/indexable.h>

namespace archie {
namespace utils {
  namespace meta {
    namespace detail {
      template <std::size_t... other>
      struct at {
        template <typename Tp>
        static constexpr decltype(auto) skip(eat<number<other>>..., Tp&&,
                                             ...) noexcept {
          return returns<Tp>{};
        }

        template <typename... Ts>
        using apply = decltype(skip(std::declval<Ts>()...));
      };
    }

    template <std::size_t n>
    using placeholder = indexable_t<detail::at, n>;

    template <std::size_t n, typename... Ts>
    struct at : placeholder<n>::template apply<Ts...> {};

    template <std::size_t n, typename... Ts>
    struct at<n, type_list<Ts...>> : at<n, Ts...> {};

    template <std::size_t n, typename... Ts>
    using at_t = eval<at<n, Ts...>>;
  }
}
}
