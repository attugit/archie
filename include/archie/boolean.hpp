#pragma once

#include <utility>
#include <archie/meta/static_constexpr_storage.hpp>

namespace archie::meta
{
  template <bool B>
  struct boolean : std::integral_constant<bool, B> {
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

namespace archie::meta
{
  template <bool B1, bool B2>
  constexpr auto operator==(boolean<B1> const&, boolean<B2> const&) noexcept
  {
    return fused::boolean<B1 == B2>;
  }

  template <bool B1, bool B2>
  constexpr auto operator==(boolean<B1> const&, std::integral_constant<bool, B2> const&) noexcept
  {
    return fused::boolean<B1 == B2>;
  }

  template <bool B1, bool B2>
  constexpr auto operator!=(boolean<B1> const& lhs, boolean<B2> const& rhs) noexcept
  {
    return fused::boolean<!(lhs == rhs)>;
  }

  template <bool B1, bool B2>
  constexpr auto operator!=(boolean<B1> const& lhs,
                            std::integral_constant<bool, B2> const& rhs) noexcept
  {
    return fused::boolean<!(lhs == rhs)>;
  }
}
