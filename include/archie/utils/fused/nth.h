#pragma once

#include <utility>
#include <archie/utils/fused/placeholder.h>
#include <config.h>

namespace archie {
namespace utils {
  namespace fused {
#if defined(HAS_VARIABLE_TEMPLATES)
    template <std::size_t n>
    constexpr placeholder<n> nth{};
#else
    template <std::size_t n, typename... Ts>
    decltype(auto) nth(Ts&&... args) {
      static_assert(n < sizeof...(Ts), "");
      return placeholder<n>{}(std::forward<Ts>(args)...);
    }
#endif
  }
}
}
