#pragma once

#include <archie/utils/meta/find.h>
#include <archie/utils/fused/nth.h>
#include <config.h>

namespace archie {
namespace utils {
  namespace fused {
#if defined(HAS_VARIABLE_TEMPLATES)
    namespace detail {
      template <typename Tp>
      struct find_ {
        template <typename... Us>
        constexpr decltype(auto) operator()(Us&&... us) const noexcept {
          return nth<meta::find_t<Tp, Us...>::value>(std::forward<Us>(us)...);
        }
      };
      template <template <typename> class F>
      struct find_if_ {
        template <typename... Us>
        constexpr decltype(auto) operator()(Us&&... us) const noexcept {
          return nth<meta::find_if_t<F, Us...>::value>(std::forward<Us>(us)...);
        }
      };
    }
    template <typename Tp>
    constexpr detail::find_<Tp> find{};
    template <template <typename> class F>
    constexpr detail::find_if_<F> find_if{};
#else
    template <typename Tp, typename... Us>
    decltype(auto) find(Us&&... us) noexcept {
      return nth<meta::find_t<Tp, Us...>::value>(std::forward<Us>(us)...);
    }

    template <template <typename> class F, typename... Us>
    decltype(auto) find_if(Us&&... us) noexcept {
      return nth<meta::find_if_t<F, Us...>::value>(std::forward<Us>(us)...);
    }
#endif
  }
}
}
