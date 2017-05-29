#pragma once

#include <utility>
#include <type_traits>

namespace archie::fused
{
  template <typename... F>
  struct overload : F... {
    constexpr overload() = default;
    template <typename... T>
    constexpr overload(T&&... t) : F(std::forward<T>(t))...
    {
    }

    using F::operator()...;
  };

  constexpr auto const make_overload = [](auto&&... fs) {
    return overload<std::decay_t<decltype(fs)>...>(std::forward<decltype(fs)>(fs)...);
  };
}
