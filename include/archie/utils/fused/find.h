#pragma once

#include <archie/utils/meta/find.h>
#include <archie/utils/fused/nth.h>

namespace archie {
namespace utils {
  namespace fused {

    template <typename Tp, typename... Us>
    decltype(auto) find(Us&&... us) noexcept {
      return nth<meta::find_t<Tp, Us...>::value>(std::forward<Us>(us)...);
    }

    template <template <typename> class F, typename... Us>
    decltype(auto) find_if(Us&&... us) noexcept {
      return nth<meta::find_if_t<F, Us...>::value>(std::forward<Us>(us)...);
    }
  }
}
}
