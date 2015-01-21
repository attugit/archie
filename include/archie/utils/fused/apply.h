#pragma once

#include <utility>
#include <archie/utils/fused/tuple.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct apply {
        template <typename F, typename... Ts>
        decltype(auto) operator()(F&& f, Ts&&... x) const
            noexcept(noexcept(std::declval<F>()(std::declval<Ts>()...))) {
          return std::forward<F>(f)(std::forward<Ts>(x)...);
        }
        template <typename F, typename... Ts>
        decltype(auto) operator()(F&& f, tuple<Ts...>& x) const
            noexcept(noexcept(std::declval<F>()(std::declval<Ts&>()...))) {
          return x.apply(std::forward<F>(f));
        }
        template <typename F, typename... Ts>
        decltype(auto) operator()(F&& f, tuple<Ts...> const& x) const noexcept(
            noexcept(std::declval<F>()(std::declval<Ts const&>()...))) {
          return x.apply(std::forward<F>(f));
        }
      };
    }
    template <typename F, typename... Ts>
    decltype(auto) apply(F&& f, Ts&&... x) noexcept(
        noexcept(std::declval<F>()(std::declval<Ts>()...))) {
      return detail::apply{}(std::forward<F>(f), std::forward<Ts>(x)...);
    }
  }
}
}
