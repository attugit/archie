#pragma once

#include <archie/traits/has_member.hpp>

namespace archie::traits
{
  namespace detail
  {
    template <typename Tp, typename Up>
    using inspect_equality_comparable = decltype(std::declval<Tp>() == std::declval<Up>());

    template <typename Tp, typename Up>
    using inspect_less_than_comparable = decltype(std::declval<Tp>() < std::declval<Up>());
  }

  template <typename Tp, typename Up>
  using is_equality_comparable =
      has_member<detail::inspect_equality_comparable, meta::type_list<Tp const&, Up const&>>;

  template <typename Tp, typename Up>
  using is_less_than_comparable =
      has_member<detail::inspect_less_than_comparable, meta::type_list<Tp const&, Up const&>>;
}
