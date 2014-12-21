#ifndef ARCHIE_UTILS_TRAITS_IS_COMPARABLE_H_INCLUDED
#define ARCHIE_UTILS_TRAITS_IS_COMPARABLE_H_INCLUDED

#include <archie/utils/meta/has_member.h>

namespace archie {
namespace utils {
  namespace traits {
    namespace detail {
      template <typename Tp, typename Up>
      using inspect_equality_comparable =
          decltype(std::declval<Tp>() == std::declval<Up>());
    }

    template <typename Tp, typename Up>
    using is_equality_comparable =
        meta::has_member<detail::inspect_equality_comparable,
                         meta::type_list<Tp const&, Up const&>>;
  }
}
}

#endif
