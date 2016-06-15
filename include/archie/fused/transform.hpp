#pragma once

#include <archie/fused/tuple.hpp>
#include <archie/fused/tuple_view.hpp>
#include <archie/fused/apply.hpp>
#include <archie/meta/static_constexpr_storage.hpp>
#include <utility>

namespace archie::fused
{
  namespace detail
  {
    struct transform_ {
      template <typename F, typename... Ts>
      decltype(auto) operator()(F&& f, Ts&&... x) const
      {
        auto make = [g = std::forward<F>(f)](auto&&... xs)
        {
          return make_tuple(g(std::forward<decltype(xs)>(xs))...);
        };
        return fused::apply(make, std::forward<Ts>(x)...);
      }
    };
    struct transform_view_ {
      template <typename F, typename... Ts>
      decltype(auto) operator()(F&& f, Ts&&... x) const
      {
        auto make = [g = std::forward<F>(f)](auto&&... xs)
        {
          return make_view(g(std::forward<decltype(xs)>(xs))...);
        };
        return fused::apply(make, std::forward<Ts>(x)...);
      }
    };
  }
  static constexpr auto const& transform = meta::instance<detail::transform_>();
  static constexpr auto const& transform_view = meta::instance<detail::transform_view_>();
}
