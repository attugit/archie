#pragma once

#include <cstddef>

namespace archie::meta
{
  struct ignore final {
    template <typename... Ts>
    constexpr ignore(Ts&&...) noexcept
    {
    }

    template <typename Tp>
    constexpr ignore const& operator=(Tp&&) const noexcept
    {
      return *this;
    }
  };

  template <typename>
  using eat = ignore;

  template <std::size_t>
  using eat_n = ignore;
}
