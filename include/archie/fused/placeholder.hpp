#pragma once

#include <utility>
#include <archie/ignore.hpp>
#include <archie/number.hpp>
#include <archie/meta/indexable.hpp>
#include <archie/meta/static_constexpr_storage.hpp>

namespace archie {
namespace fused {
  namespace detail {
    template <typename... other>
    struct placeholder {
      template <typename Tp, typename... Us>
      constexpr decltype(auto) operator()(meta::eat<other>..., Tp&& x, Us&&...) const noexcept
      {
        return std::forward<Tp>(x);
      }
    };
  }

  template <std::size_t n>
  struct placeholder : meta::indexable_t<detail::placeholder, n> {
  };

  template <typename F, std::size_t... ids>
  decltype(auto) reorder(F f, placeholder<ids>... xs)
  {
    return [f, xs...](auto&&... ys) { return f(xs(ys...)...); };
  }

  template <std::size_t N>
  static constexpr auto const& nth = meta::instance<placeholder<N>>();

  inline namespace placeholders {
    static constexpr auto const& _0 = fused::nth<0>;
    static constexpr auto const& _1 = fused::nth<1>;
    static constexpr auto const& _2 = fused::nth<2>;
    static constexpr auto const& _3 = fused::nth<3>;
    static constexpr auto const& _4 = fused::nth<4>;
    static constexpr auto const& _5 = fused::nth<5>;
    static constexpr auto const& _6 = fused::nth<6>;
    static constexpr auto const& _7 = fused::nth<7>;
    static constexpr auto const& _8 = fused::nth<8>;
    static constexpr auto const& _9 = fused::nth<9>;
  }
}
}
