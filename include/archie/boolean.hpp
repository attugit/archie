#pragma once

#include <utility>
#include <archie/meta/comparable.hpp>

namespace archie::meta {
  template <bool B>
  struct boolean: std::integral_constant<bool, B>, comparable<boolean<B>>, comparable<std::integral_constant<bool, B>> {};

  using true_t = boolean<true>;
  using false_t = boolean<false>;
}

namespace archie::fused
{
  static constexpr auto const True = meta::true_t{};
  static constexpr auto const False = meta::false_t{};
}
