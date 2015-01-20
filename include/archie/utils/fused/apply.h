#pragma once

#include <utility>
#include <archie/utils/fused/tuple.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      template <typename... Ts>
      struct apply {
        template <typename F>
        decltype(auto) operator()(F&& f, Ts&&... x) noexcept(
            noexcept(std::declval<F>()(std::declval<Ts>()...))) {
          return std::forward<F>(f)(std::forward<Ts>(x)...);
        }
      };

      template <typename... Ts>
      struct apply<tuple<Ts...>> {
        template <typename F>
        decltype(auto) operator()(F&& f, tuple<Ts...>& x) const
            noexcept(noexcept(std::declval<F>()(std::declval<Ts&>()...))) {
          return x.apply(f);
        }
        template <typename F>
        decltype(auto) operator()(F&& f, tuple<Ts...> const& x) const noexcept(
            noexcept(std::declval<F>()(std::declval<Ts const&>()...))) {
          return x.apply(f);
        }
      };
    }
    template <typename F, typename... Ts>
    decltype(auto) apply(F&& f, Ts&&... x) noexcept(
        noexcept(std::declval<F>()(std::declval<Ts>()...))) {
      return detail::apply<std::decay_t<Ts>...>{}(std::forward<F>(f),
                                                  std::forward<Ts>(x)...);
    }
  }
}
}
