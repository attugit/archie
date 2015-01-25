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
                                  Us&&...) const noexcept -> decltype(x) {
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
      constexpr placeholder<0> _0{};
      constexpr placeholder<1> _1{};
      constexpr placeholder<2> _2{};
      constexpr placeholder<3> _3{};
      constexpr placeholder<4> _4{};
      constexpr placeholder<5> _5{};
      constexpr placeholder<6> _6{};
      constexpr placeholder<7> _7{};
      constexpr placeholder<8> _8{};
      constexpr placeholder<9> _9{};
    }
  }
}
}
