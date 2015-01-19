#pragma once

#include <utility>
#include <archie/utils/fused/placeholder.h>

namespace archie {
namespace utils {
  namespace fused {
    template <std::size_t n, typename... Ts>
    decltype(auto) nth(Ts&&... args) {
      static_assert(n < sizeof...(Ts), "");
      return placeholder<n>{}(std::forward<Ts>(args)...);
    }
  }
}
}
