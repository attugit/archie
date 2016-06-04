#pragma once

#include <utility>
#include <archie/meta/static_constexpr_storage.hpp>
#include <archie/fused/apply.hpp>
#include <archie/fused/take.hpp>

namespace archie::fused
{
  namespace detail
  {
    template <std::size_t N>
    struct slice_ {
      template <typename T>
      constexpr decltype(auto) operator()(T&& t) const
      {
        return fused::apply(fused::take<N>, std::forward<T>(t));
      }
    };
  }

  template <std::size_t N>
  static constexpr auto const& slice = meta::instance<detail::slice_<N>>();
}
