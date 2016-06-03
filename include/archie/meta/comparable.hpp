#pragma once

#include <archie/boolean.hpp>

namespace archie::meta
{
  template <typename>
  struct comparable {
  };

  template <typename T>
  constexpr auto operator==(comparable<T> const&, comparable<T> const&) noexcept
  {
    return fused::True;
  }

  template <typename T, typename U>
  constexpr auto operator==(comparable<T> const&, comparable<U> const&) noexcept
  {
    return fused::False;
  }

  template <typename T>
  constexpr auto operator!=(comparable<T> const&, comparable<T> const&) noexcept
  {
    return fused::False;
  }

  template <typename T, typename U>
  constexpr bool operator!=(comparable<T> const&, comparable<U> const&) noexcept
  {
    return fused::True;
  }
}
