#ifndef ARCHIE_UTILS_TRAITS_IS_CONSTRUCTIBLE_H_INCLUDED
#define ARCHIE_UTILS_TRAITS_IS_CONSTRUCTIBLE_H_INCLUDED

#include <archie/utils/meta/has_member.h>

namespace archie {
namespace utils {
  namespace traits {
    namespace detail {
      template <typename Tp, typename... Us>
      using inspect_constructible = decltype(Tp{std::declval<Us>()...});
    }
    template <typename Tp, typename... Us>
    using is_constructible = meta::has_member<detail::inspect_constructible,
                                              meta::type_list<Tp, Us...>>;

    template <typename Tp>
    using is_default_constructible = is_constructible<Tp>;
  }
}
}

#endif
