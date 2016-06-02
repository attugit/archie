#pragma once

namespace archie::meta
{
  template <typename>
  struct comparable {
  };

  template <typename T>
  constexpr bool operator==(comparable<T> const&, comparable<T> const&) noexcept
  {
    return true;
  }

  template <typename T, typename U>
  constexpr bool operator==(comparable<T> const&, comparable<U> const&) noexcept
  {
    return false;
  }

  template <typename T>
  constexpr bool operator!=(comparable<T> const&, comparable<T> const&) noexcept
  {
    return false;
  }

  template <typename T, typename U>
  constexpr bool operator!=(comparable<T> const&, comparable<U> const&) noexcept
  {
    return true;
  }
}
