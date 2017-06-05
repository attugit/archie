#pragma once

#include <utility>
#include <archie/ignore.hpp>

namespace archie::fused
{
  constexpr auto const back = [](auto&& t, auto&&... us) -> decltype(auto) {
    return [](meta::eat<decltype(us)>..., auto&& x) -> decltype(x) {
      return std::forward<decltype(x)>(x);
    }(std::forward<decltype(t)>(t), std::forward<decltype(us)>(us)...);
  };
}
