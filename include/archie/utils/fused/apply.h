#pragma once

#include <utility>

namespace archie {
namespace utils {
  namespace fused {
    template <typename Func, typename... Ts>
    constexpr decltype(auto) apply(Func&& func, Ts&&... ts) noexcept(
        noexcept(std::declval<Func>()(std::declval<Ts>()...))) {
      return std::forward<Func>(func)(std::forward<Ts>(ts)...);
    }
  }
}
}
