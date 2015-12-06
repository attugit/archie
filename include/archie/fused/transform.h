#pragma once

#include <archie/fused/tuple.h>
#include <archie/fused/tuple_view.h>
#include <archie/fused/apply.h>
#include <archie/meta/static_constexpr_storage.h>
#include <utility>

namespace archie {
namespace fused {
  namespace detail {
    struct transform_ {
      template <typename F, typename... Ts>
      decltype(auto) operator()(F&& f, Ts&&... x) const {
        auto make = [g = std::forward<F>(f)](auto&&... xs) {
          return make_tuple(g(std::forward<decltype(xs)>(xs))...);
        };
        return fused::apply(make, std::forward<Ts>(x)...);
      }
    };
    struct transform_view_ {
      template <typename F, typename... Ts>
      decltype(auto) operator()(F&& f, Ts&&... x) const {
        auto make = [g = std::forward<F>(f)](auto&&... xs) {
          return make_view(g(std::forward<decltype(xs)>(xs))...);
        };
        return fused::apply(make, std::forward<Ts>(x)...);
      }
    };
  }
  static constexpr auto const& transform = meta::instance<detail::transform_>();
  static constexpr auto const& transform_view = meta::instance<detail::transform_view_>();
}
}
