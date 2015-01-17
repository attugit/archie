#pragma once

#include <archie/utils/meta/has_member.h>

namespace archie {
namespace utils {
  namespace traits {
    namespace detail {
      template <typename Tp, typename Up>
      using inspect_equality_comparable =
          decltype(std::declval<Tp>() == std::declval<Up>());

      template <typename Tp, typename Up>
      using inspect_less_than_comparable =
          decltype(std::declval<Tp>() < std::declval<Up>());
    }

    template <typename Tp, typename Up>
    using is_equality_comparable =
        meta::has_member<detail::inspect_equality_comparable,
                         meta::type_list<Tp const&, Up const&>>;

    template <typename Tp, typename Up>
    using is_less_than_comparable =
        meta::has_member<detail::inspect_less_than_comparable,
                         meta::type_list<Tp const&, Up const&>>;
  }
}
}
