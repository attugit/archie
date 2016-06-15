#pragma once

#include <utility>
#include <archie/meta.hpp>
#include <archie/meta/indexable.hpp>
#include <archie/fused/tuple.hpp>
#include <archie/fused/nth.hpp>

namespace archie::fused
{
  namespace detail
  {
    template <typename... ids>
    struct take_ {
      template <typename... Ts>
      constexpr decltype(auto) operator()(Ts&&... ts) const
      {
        return make_tuple(fused::nth<ids::value>(std::forward<Ts>(ts)...)...);
      }
    };
    template <std::size_t N>
    using indexed_take_ = meta::indexable_t<detail::take_, N>;
  }

  template <std::size_t N>
  static constexpr auto const& take = meta::instance<detail::indexed_take_<N>>();
}
