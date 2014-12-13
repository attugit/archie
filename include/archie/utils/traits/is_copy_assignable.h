#ifndef ARCHIE_UTILS_TRAITS_IS_COPY_ASSIGNABLE_H_INCLUDED
#define ARCHIE_UTILS_TRAITS_IS_COPY_ASSIGNABLE_H_INCLUDED

#include <archie/utils/meta/has_member.h>

namespace archie {
namespace utils {
  namespace traits {
    namespace detail {
      template <typename Tp>
      using inspect_copy_assignable =
          decltype(std::declval<Tp&>() = std::declval<Tp const&>());
    }

    template <typename Tp>
    using is_copy_assignable =
        meta::has_member<detail::inspect_copy_assignable, meta::type_list<Tp>>;
  }
}
}

#endif
