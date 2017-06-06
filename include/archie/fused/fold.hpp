#pragma once

#include <utility>
#include <archie/fused/overload.hpp>

namespace archie::fused
{
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

  constexpr auto const& make_fold = fused::overload(
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
