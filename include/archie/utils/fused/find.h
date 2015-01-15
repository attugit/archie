#pragma once

#include <archie/utils/meta/find.h>
#include <archie/utils/fused/nth.h>

namespace archie {
namespace utils {
  namespace fused {

    template <typename Tp, typename... Us>
    decltype(auto) find(Us&&... us) noexcept {
      using idx = meta::find_t<Tp, Us...>;
      static_assert(idx::value < sizeof...(Us), "");
      return nth<idx::value>(std::forward<Us>(us)...);
    }
  }
}
}
