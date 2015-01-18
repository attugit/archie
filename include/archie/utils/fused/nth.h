#pragma once

#include <utility>
#include <archie/utils/meta/ignore.h>
#include <archie/utils/meta/number.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      template <std::size_t n, typename = std::make_index_sequence<n>>
      struct nth;

      template <std::size_t n, std::size_t... other>
      struct nth<n, std::index_sequence<other...>> {
        template <typename Tp, typename... Us>
        constexpr auto operator()(meta::eat<meta::number<other>>..., Tp&& x,
                                  Us&&...) noexcept -> decltype(x) {
          return std::forward<Tp>(x);
        }
      };
    }

    template <std::size_t n, typename... Ts>
    decltype(auto) nth(Ts&&... args) {
      static_assert(n < sizeof...(Ts), "");
      return detail::nth<n>{}(std::forward<Ts>(args)...);
    }
  }
}
}
