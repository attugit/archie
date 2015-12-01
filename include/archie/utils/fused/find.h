#pragma once

#include <archie/utils/meta/find.h>
#include <archie/utils/meta/static_constexpr_storage.h>
#include <archie/utils/fused/nth.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      template <typename Tp>
      struct find_ {
        template <typename... Us>
        constexpr decltype(auto) operator()(Us&&... us) const noexcept {
          return fused::nth<meta::find_t<Tp, Us...>::value>(std::forward<Us>(us)...);
        }
      };
      template <template <typename> class F>
      struct find_if_ {
        template <typename... Us>
        constexpr decltype(auto) operator()(Us&&... us) const noexcept {
          return fused::nth<meta::find_if_t<F, Us...>::value>(std::forward<Us>(us)...);
        }
      };
    }
    template <typename T>
    static constexpr auto const& find = meta::instance<detail::find_<T>>();
    template <template <typename> class F>
    static constexpr auto const& find_if = meta::instance<detail::find_if_<F>>();
  }
}
}
