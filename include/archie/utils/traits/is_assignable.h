#pragma once

#include <archie/utils/meta/has_member.h>

namespace archie {
namespace utils {
  namespace traits {
    namespace detail {
      template <typename Tp, typename Up>
      using inspect_assignable =
          decltype(std::declval<Tp>() = std::declval<Up>());
    }

    template <typename Tp, typename Up>
    using is_assignable =
        meta::has_member<detail::inspect_assignable, meta::type_list<Tp&, Up>>;

    template <typename Tp>
    using is_copy_assignable = is_assignable<Tp, Tp const&>;

    template <typename Tp>
    using is_move_assignable = is_assignable<Tp, Tp&&>;
  }
}
}
