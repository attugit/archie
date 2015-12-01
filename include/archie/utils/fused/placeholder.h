#pragma once

#include <utility>
#include <archie/utils/meta/ignore.h>
#include <archie/utils/meta/number.h>
#include <archie/utils/meta/indexable.h>
#include <archie/utils/meta/variable_template.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      template <std::size_t... other>
      struct placeholder {
        template <typename Tp, typename... Us>
        constexpr decltype(auto) operator()(meta::eat_n<other>..., Tp&& x, Us&&...) const
            noexcept {
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

    DECL_VARTEMPL(nth, placeholder, std::size_t);

    inline namespace placeholders {
      constexpr auto const& _0 = VARTEMPL(fused::nth, 0);
      constexpr auto const& _1 = VARTEMPL(fused::nth, 1);
      constexpr auto const& _2 = VARTEMPL(fused::nth, 2);
      constexpr auto const& _3 = VARTEMPL(fused::nth, 3);
      constexpr auto const& _4 = VARTEMPL(fused::nth, 4);
      constexpr auto const& _5 = VARTEMPL(fused::nth, 5);
      constexpr auto const& _6 = VARTEMPL(fused::nth, 6);
      constexpr auto const& _7 = VARTEMPL(fused::nth, 7);
      constexpr auto const& _8 = VARTEMPL(fused::nth, 8);
      constexpr auto const& _9 = VARTEMPL(fused::nth, 9);
    }
  }
}
}
