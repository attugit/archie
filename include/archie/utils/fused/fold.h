#pragma once

#include <utility>
#include <archie/utils/meta/static_constexpr_storage.h>

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
        constexpr decltype(auto) impl(F&& f, S&& arg_state, X&& arg_x) const {
          return std::forward<F>(f)(std::forward<S>(arg_state), std::forward<X>(arg_x));
        }
        template <typename F, typename S, typename X, typename... Ys>
        constexpr decltype(auto) impl(F&& f, S&& arg_state, X&& arg_x, Ys&&... ys) const {
          return impl(std::forward<F>(f), f(std::forward<S>(arg_state), std::forward<X>(arg_x)),
                      std::forward<Ys>(ys)...);
        }
      };
      struct greedy_fold_ {
        template <typename F, typename S, typename... Xs>
        decltype(auto) operator()(F const& f, S const& s, Xs const&... xs) const {
          return impl(f, s, xs...);
        }

      private:
        template <typename F, typename S, typename X>
        decltype(auto) impl(F const& f, S const& s, X const& x) const {
          return f(s, x);
        }
        template <typename F, typename S, typename X, typename... Ys>
        decltype(auto) impl(F const& f, S const& s, X const& x, Ys const&... ys) const {
          return impl(f, f(s, x, ys...), ys...);
        }
      };
    }
    static constexpr auto const& fold = meta::instance<detail::fold_>();
    static constexpr auto const& greedy_fold = meta::instance<detail::greedy_fold_>();
    namespace detail {
      struct make_fold_ {
      private:
        template <typename F>
        struct impl_no_state {
          F const func;
          constexpr impl_no_state(F const& f) : func(f) {}
          template <typename... Ts>
          constexpr decltype(auto) operator()(Ts&&... ts) const {
            return fused::fold(func, std::forward<Ts>(ts)...);
          }
        };
        template <typename F, typename S>
        struct impl_with_state {
          F const func;
          S const state;
          constexpr impl_with_state(F const& f, S const& s) : func(f), state(s) {}
          template <typename... Ts>
          constexpr decltype(auto) operator()(Ts&&... ts) const {
            return fused::fold(func, state, std::forward<Ts>(ts)...);
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
    static constexpr auto const& make_fold = meta::instance<detail::make_fold_>();
  }
}
}
