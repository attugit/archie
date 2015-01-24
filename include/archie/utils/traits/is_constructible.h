#pragma once

#include <type_traits>
#include <config.h>
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

#if defined(COMPILER_GCC)
    template <typename Tp>
    using is_trivially_copy_constructible =
        std::has_trivial_copy_constructor<Tp>;
#elif defined(COMPILER_CLANG)
    template <typename Tp>
    using is_trivially_copy_constructible =
        std::is_trivially_copy_constructible<Tp>;
#endif
  }
}
}
