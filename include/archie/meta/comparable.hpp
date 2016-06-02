#pragma once

namespace archie::meta
{
  template <typename T>
  struct comparable {
    constexpr bool operator==(T const&) const noexcept { return true; }
    template <typename U>
    constexpr bool operator==(U const&) const noexcept
    {
      return false;
    }
    template <typename U>
    constexpr bool operator!=(U const& u) const noexcept
    {
      return !(*this == u);
    }
  };
}
