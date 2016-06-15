#pragma once

#include <archie/traits/has_member.hpp>

namespace archie::traits
{
  namespace detail
  {
    template <typename Tp>
    struct convert_helper {
      static void implicit(Tp);
    };

    template <typename Tp, typename Up>
    using inspect_convertible = decltype(convert_helper<Up>::implicit(std::declval<Tp>()));
  }
  template <typename Tp, typename Up>
  using is_convertible = has_member<detail::inspect_convertible, meta::type_list<Tp, Up>>;
}
