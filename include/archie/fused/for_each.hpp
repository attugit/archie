#pragma once

#include <utility>
#include <functional>

namespace archie::fused
{
  constexpr auto const for_each = [](auto&& f, auto&&... args) -> decltype(auto) {
    int skip[]{0, (std::ref(f)(std::forward<decltype(args)>(args)), void(), 0)...};
    (void)skip;
    return std::forward<decltype(f)>(f);
  };
}
