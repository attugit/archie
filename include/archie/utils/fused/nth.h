#pragma once

#include <utility>
#include <archie/utils/fused/placeholder.h>
#include <archie/utils/meta/variable_template.h>

namespace archie {
namespace utils {
  namespace fused {
#if defined(HAS_VARIABLE_TEMPLATES)
    template <std::size_t n>
    constexpr auto const nth = placeholder<n>{};
#else
    template <std::size_t n>
    struct nth_v : meta::variable_template<placeholder<n>> {};

    template <std::size_t n, typename... Ts>
    decltype(auto) nth(Ts&&... args) {
      return nth_v<n>::value(std::forward<Ts>(args)...);
    }
#endif
  }
}
}
