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
      struct placeholder {
        template <typename Tp, typename... Us>
        constexpr auto operator()(meta::eat<meta::number<other>>..., Tp&& x,
                                  Us&&...) noexcept -> decltype(x) {
          return std::forward<Tp>(x);
        }
      };
    }

    template <std::size_t n>
    struct placeholder : meta::indexable_t<detail::placeholder, n> {};

    template <typename F, std::size_t... ids>
    decltype(auto) reorder(F f, placeholder<ids>... xs) {
      return [f, xs...](auto&&... ys) { return f(xs(ys...)...); };
    }
  }
}
}
