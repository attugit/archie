#pragma once

#include <utility>
#include <archie/meta/static_constexpr_storage.hpp>
#include <archie/fused/overload.hpp>
#include <archie/boolean.hpp>

namespace archie::fused
{
  namespace detail
  {
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
  static constexpr auto const& greedy_fold = meta::instance<detail::greedy_fold_>();

  constexpr auto const fold = [](auto const& func, auto&& init, auto&&... xs) -> decltype(auto) {
    if
      constexpr(sizeof...(xs))
      {
        auto const recursion = [&func](auto self, auto&& state, auto&& a,
                                       auto&&... bs) -> decltype(auto) {
          if
            constexpr(sizeof...(bs))
            {
              return self(self,
                          func(std::forward<decltype(state)>(state), std::forward<decltype(a)>(a)),
                          std::forward<decltype(bs)>(bs)...);
            }
          else
          {
            (void)self;
            return func(std::forward<decltype(state)>(state), std::forward<decltype(a)>(a));
          }
        };
        return recursion(recursion, std::forward<decltype(init)>(init),
                         std::forward<decltype(xs)>(xs)...);
      }
    else
    {
      return std::forward<decltype(init)>(init);
    }
  };

  constexpr auto const& make_fold = fused::make_overload(
      [](auto const& f) {
        return [f](auto&&... xs) { return fused::fold(f, std::forward<decltype(xs)>(xs)...); };
      },
      [](auto const& f, auto&& state) {
        return [ f, state = std::forward<decltype(state)>(state) ](auto&&... xs)
        {
          return fused::fold(f, std::forward<decltype(state)>(state),
                             std::forward<decltype(xs)>(xs)...);
        };
      });
}
