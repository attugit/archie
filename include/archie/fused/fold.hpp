#pragma once

#include <utility>
#include <archie/meta/static_constexpr_storage.hpp>
#include <archie/fused/static_if.hpp>
#include <archie/boolean.hpp>

namespace archie::fused
{
  namespace detail
  {
    struct fold_ {
      template <typename F, typename S, typename... Xs>
      constexpr decltype(auto) operator()(F const& func, S&& acc, Xs&&... xs) const
      {
        return static_if(fused::boolean<(sizeof...(Xs) > 1)>)
            .then([this](auto const& f, auto&& state, auto&& x_, auto&&... y) -> decltype(auto) {
              return this->operator()(
                  f, f(std::forward<decltype(state)>(state), std::forward<decltype(x_)>(x_)),
                  std::forward<decltype(y)>(y)...);
            })
            .else_([](auto const& f, auto&& state, auto&& x_) -> decltype(auto) {
              return f(std::forward<decltype(state)>(state), std::forward<decltype(x_)>(x_));
            })(func, std::forward<S>(acc), std::forward<Xs>(xs)...);
      }
    };
    struct greedy_fold_ {
      template <typename F, typename S, typename... Xs>
      decltype(auto) operator()(F const& f, S const& s, Xs const&... xs) const
      {
        return impl(f, s, xs...);
      }

    private:
      template <typename F, typename S, typename X>
      decltype(auto) impl(F const& f, S const& s, X const& x) const
      {
        return f(s, x);
      }
      template <typename F, typename S, typename X, typename... Ys>
      decltype(auto) impl(F const& f, S const& s, X const& x, Ys const&... ys) const
      {
        return impl(f, f(s, x, ys...), ys...);
      }
    };
  }
  static constexpr auto const& fold = meta::instance<detail::fold_>();
  static constexpr auto const& greedy_fold = meta::instance<detail::greedy_fold_>();
  namespace detail
  {
    struct make_fold_ {
    private:
      template <typename F>
      struct impl_no_state {
        F const func;
        constexpr impl_no_state(F const& f) : func(f) {}
        template <typename... Ts>
        constexpr decltype(auto) operator()(Ts&&... ts) const
        {
          return fused::fold(func, std::forward<Ts>(ts)...);
        }
      };
      template <typename F, typename S>
      struct impl_with_state {
        F const func;
        S const state;
        constexpr impl_with_state(F const& f, S const& s) : func(f), state(s) {}
        template <typename... Ts>
        constexpr decltype(auto) operator()(Ts&&... ts) const
        {
          return fused::fold(func, state, std::forward<Ts>(ts)...);
        }
      };

    public:
      template <typename F>
      constexpr decltype(auto) operator()(F const& f) const
      {
        return impl_no_state<F>{f};
      }
      template <typename F, typename S>
      constexpr decltype(auto) operator()(F const& f, S const& s) const
      {
        return impl_with_state<F, S>{f, s};
      }
    };
  }
  static constexpr auto const& make_fold = meta::instance<detail::make_fold_>();
}
