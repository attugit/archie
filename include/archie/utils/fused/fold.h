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
      private:
        template <typename F>
        struct impl_no_state {
          F const f;
          constexpr impl_no_state(F const& f) : f(f) {}
          template <typename... Ts>
          constexpr decltype(auto) operator()(Ts&&... ts) const {
            return fused::fold(f, std::forward<Ts>(ts)...);
          }
        };
        template <typename F, typename S>
        struct impl_with_state {
          F const f;
          S const state;
          constexpr impl_with_state(F const& f, S const& s) : f(f), state(s) {}
          template <typename... Ts>
          constexpr decltype(auto) operator()(Ts&&... ts) const {
            return fused::fold(f, state, std::forward<Ts>(ts)...);
          }
        };

      public:
        template <typename F>
        constexpr decltype(auto) operator()(F const& f) const {
          return impl_no_state<F>{f};
        }
        template <typename F, typename S>
        constexpr decltype(auto) operator()(F const& f, S const& s) const {
          return impl_with_state<F, S>{f, s};
        }
      };
    }
    constexpr auto const make_fold = detail::make_fold_{};
  }
}
}
