#pragma once

#include <utility>
#include <archie/utils/meta/ignore.h>
#include <archie/utils/meta/number.h>
#include <archie/utils/meta/indexable.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      template <std::size_t... other>
      struct nth {
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
      return meta::indexable_t<detail::nth, n>{}(std::forward<Ts>(args)...);
    }
  }
}
}
