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

    inline namespace placeholders {
      auto _0 = fused::placeholder<0>{};
      auto _1 = fused::placeholder<1>{};
      auto _2 = fused::placeholder<2>{};
      auto _3 = fused::placeholder<3>{};
      auto _4 = fused::placeholder<4>{};
      auto _5 = fused::placeholder<5>{};
      auto _6 = fused::placeholder<6>{};
      auto _7 = fused::placeholder<7>{};
      auto _8 = fused::placeholder<8>{};
      auto _9 = fused::placeholder<9>{};
    }
  }
}
}
