#pragma once

#include <utility>
#include <archie/meta/static_constexpr_storage.hpp>

namespace archie::meta
{
  template <std::size_t N>
  struct number : std::integral_constant<std::size_t, N> {
  };
}

namespace archie::fused
{
  template <std::size_t N>
  static constexpr auto const& number = meta::instance<meta::number<N>>();
}
