#pragma once

#include <archie/traits/has_member.hpp>

namespace archie::traits
{
  namespace detail
  {
    template <typename Tp, typename Up>
    using inspect_assignable = decltype(std::declval<Tp>() = std::declval<Up>());
  }

  template <typename Tp, typename Up>
  using is_assignable = has_member<detail::inspect_assignable, meta::type_list<Tp&, Up>>;

  template <typename Tp>
  using is_copy_assignable = is_assignable<Tp, Tp const&>;

  template <typename Tp>
  using is_move_assignable = is_assignable<Tp, Tp&&>;
}
