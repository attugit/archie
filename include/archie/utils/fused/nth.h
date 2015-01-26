#pragma once

#include <utility>
#include <archie/utils/fused/placeholder.h>

namespace archie {
namespace utils {
  namespace fused {
#if defined(COMPILER_CLANG)
    template <std::size_t n>
    constexpr meta::indexable_t<detail::placeholder, n> nth{};
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
