#pragma once

#include <utility>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct fold_ {
        template <typename F, typename S, typename... Xs>
        constexpr decltype(auto) operator()(F&& f, S&& s, Xs&&... xs) const {
          return impl(std::forward<F>(f), std::forward<S>(s), std::forward<Xs>(xs)...);
        }

      private:
        template <typename F, typename S, typename X>
        constexpr decltype(auto) impl(F&& f, S&& s, X&& x) const {
          return std::forward<F>(f)(std::forward<S>(s), std::forward<X>(x));
        }
        template <typename F, typename S, typename X, typename... Ys>
        constexpr decltype(auto) impl(F&& f, S&& s, X&& x, Ys&&... ys) const {
          return impl(std::forward<F>(f),
                      f(std::forward<S>(s), std::forward<X>(x)),
                      std::forward<Ys>(ys)...);
        }
      };
    }
    constexpr auto const fold = detail::fold_{};
    namespace detail {
      struct make_fold_ {
        template <typename F>
        constexpr decltype(auto) operator()(F&& f) const {
          return [g = std::forward<F>(f)](auto&&... xs) {
            return fused::fold(std::forward<decltype(g)>(g), std::forward<decltype(xs)>(xs)...);
          };
        }
        template <typename F, typename S>
        constexpr decltype(auto) operator()(F&& f, S&& s) const {
          return [ g = std::forward<F>(f), state = std::forward<S>(s) ](auto&&... xs) {
            return fused::fold(std::forward<decltype(g)>(g),
                               std::forward<decltype(state)>(state),
                               std::forward<decltype(xs)>(xs)...);
          };
        }
      };
    }
    constexpr auto const make_fold = detail::make_fold_{};
  }
}
}
