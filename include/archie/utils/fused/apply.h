#pragma once

#include <utility>
#include <archie/utils/meta/requires.h>
#include <archie/utils/fused/tuple.h>
#include <archie/utils/traits/is_fused_tuple.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct apply {
        template <typename F, typename Tp, typename... Us>
        decltype(auto) operator()(F&& f, Tp&& x, Us&&... y) const
            noexcept(noexcept(std::declval<F>()(std::declval<Tp>(),
                                                std::declval<Us>()...))) {
          return std::forward<F>(f)(std::forward<Tp>(x),
                                    std::forward<Us>(y)...);
        }
        template <typename F, typename Tp,
                  meta::requires<traits::is_fused_tuple<std::decay_t<Tp>>>...>
        decltype(auto) operator()(F&& f, Tp&& x) const {
          return std::forward<Tp>(x).apply(std::forward<F>(f));
        }
        template <
            typename F, typename Tp,
            meta::requires_none<traits::is_fused_tuple<std::decay_t<Tp>>>...>
        decltype(auto) operator()(F&& f, Tp&& x) const
            noexcept(noexcept(std::declval<F>()(std::declval<Tp>()))) {
          return std::forward<F>(f)(std::forward<Tp>(x));
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
