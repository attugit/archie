#pragma once

#include <utility>
#include <archie/meta/static_constexpr_storage.hpp>
#include <archie/meta/comparable.hpp>

namespace archie::meta
{
  template <bool B>
  struct boolean : std::integral_constant<bool, B>,
                   comparable<boolean<B>>,
                   comparable<std::integral_constant<bool, B>> {
  };

  using true_t = boolean<true>;
  using false_t = boolean<false>;
}

namespace archie::fused
{
  template <bool B>
  static constexpr auto const& boolean = meta::instance<meta::boolean<B>>();
  static constexpr auto const True = boolean<true>;
  static constexpr auto const False = boolean<false>;
}
