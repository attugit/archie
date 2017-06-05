#pragma once

#include <archie/boolean.hpp>

namespace archie::meta
{
  template <typename... Ts>
  constexpr bool const all = (Ts::value && ...);

  template <typename... Ts>
  constexpr bool const any = (Ts::value || ...);

  template <typename... Ts>
  constexpr bool const none = !any<Ts...>;

  template <typename T>
  constexpr bool const no = none<T>;
}
