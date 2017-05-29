#pragma once

#include <utility>
#include <type_traits>

namespace archie::fused
{
  template <typename... F>
  struct conditional : F... {
    constexpr conditional() = default;
    template <typename... T>
    constexpr conditional(T&&... t) : F(std::forward<T>(t))...
    {
    }

    using F::operator()...;
  };

  constexpr auto const make_conditional = [](auto&&... fs) {
    return conditional<std::decay_t<decltype(fs)>...>(std::forward<decltype(fs)>(fs)...);
  };
}
