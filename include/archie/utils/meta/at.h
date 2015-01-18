#pragma once

#include <utility>
#include <archie/utils/meta/ignore.h>
#include <archie/utils/meta/returns.h>
#include <archie/utils/meta/number.h>
#include <archie/utils/meta/type_list.h>

namespace archie {
namespace utils {
  namespace meta {
    namespace detail {
      template <std::size_t n, typename = std::make_index_sequence<n>>
      struct at;

      template <std::size_t n, std::size_t... other>
      struct at<n, std::index_sequence<other...>> {
      private:
        template <typename Tp>
        static constexpr Tp skip(eat<number<other>>..., Tp&&, ...) noexcept;

      public:
        template <typename... Ts>
        using apply = returns<decltype(skip(std::declval<Ts>()...))>;
      };
    }

    template <std::size_t n>
    using placeholder = detail::at<n>;

    template <std::size_t n, typename... Ts>
    struct at : detail::at<n>::template apply<Ts...> {};

    template <std::size_t n, typename... Ts>
    struct at<n, type_list<Ts...>> : at<n, Ts...> {};

    template <std::size_t n, typename... Ts>
    using at_t = eval<at<n, Ts...>>;
  }
}
}
