#pragma once

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
    DECL_VTEMPL(find, detail::find_, typename);
    DECL_VTEMPL(find_if, detail::find_if_, template <typename> class);
  }
}
}
