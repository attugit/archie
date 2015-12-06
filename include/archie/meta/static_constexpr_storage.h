#pragma once

namespace archie {
namespace meta {
  template <typename T>
  struct static_constexpr_storage {
    static constexpr T const value{};
  };

  template <typename T>
  constexpr T const static_constexpr_storage<T>::value;

  template <typename T>
  constexpr auto const& instance() noexcept {
    return static_constexpr_storage<T>::value;
  }
}
}
