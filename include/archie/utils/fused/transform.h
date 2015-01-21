#pragma once

#include <archie/utils/fused/tuple.h>
#include <archie/utils/fused/apply.h>
#include <utility>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct transform {
        template <typename F, typename... Ts>
        decltype(auto) operator()(F&& f, Ts&&... x) const {
          return make_tuple(std::forward<F>(f)(std::forward<Ts>(x))...);
        }
        template <typename F, typename... Ts>
        decltype(auto) operator()(F&& f, tuple<Ts...>& x) const {
          auto make = [g = std::forward<F>(f)](auto&... xs) {
            return make_tuple(g(xs)...);
          };
          return fused::apply(make, x);
        }
        template <typename F, typename... Ts>
        decltype(auto) operator()(F&& f, tuple<Ts...> const& x) const {
          auto make = [g = std::forward<F>(f)](auto const&... xs) {
            return make_tuple(g(xs)...);
          };
          return fused::apply(make, x);
        }
      };
    }
    template <typename F, typename... Ts>
    decltype(auto) transform(F&& f, Ts&&... x) {
      return detail::transform{}(std::forward<F>(f), std::forward<Ts>(x)...);
    }
  }
}
}
