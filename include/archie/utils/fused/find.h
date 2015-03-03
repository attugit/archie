#pragma once

#include <config.h>
#include <archie/utils/meta/find.h>
#include <archie/utils/meta/variable_template.h>
#include <archie/utils/fused/nth.h>

namespace archie {
namespace utils {
  namespace fused {
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
#if defined(HAS_VARIABLE_TEMPLATES)
    template <typename Tp>
    constexpr auto const find = detail::find_<Tp>{};
    template <template <typename> class F>
    constexpr auto const find_if = detail::find_if_<F>{};
#else

    template <typename Tp>
    struct find_v : meta::variable_template<detail::find_<Tp>> {};

    template <typename Tp, typename... Us>
    decltype(auto) find(Us&&... us) noexcept {
      return fused::find_v<Tp>::value(std::forward<Us>(us)...);
    }

    template <template <typename> class F>
    struct find_if_v : meta::variable_template<detail::find_if_<F>> {};

    template <template <typename> class F, typename... Us>
    decltype(auto) find_if(Us&&... us) noexcept {
      return fused::find_if_v<F>::value(std::forward<Us>(us)...);
    }
#endif
  }
}
}
