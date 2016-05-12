#pragma once

#include <type_traits>
#include <archie/traits/has_member.hpp>

namespace archie {
namespace traits {
  namespace detail {
    template <typename Tp, typename... Us>
    using inspect_constructible = decltype(Tp{std::declval<Us>()...});
  }
  template <typename Tp, typename... Us>
  using is_constructible = has_member<detail::inspect_constructible, meta::type_list<Tp, Us...>>;

  template <typename Tp>
  using is_default_constructible = is_constructible<Tp>;

  template <typename Tp>
  using is_move_constructible = is_constructible<Tp, Tp&&>;

  template <typename Tp>
  using is_trivially_copy_constructible = std::is_trivially_copy_constructible<Tp>;
}
}
