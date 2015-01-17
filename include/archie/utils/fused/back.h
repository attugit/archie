#pragma once

#include <utility>
#include <archie/utils/meta/ignore.h>

namespace archie {
namespace utils {
  namespace fused {

    template <typename Tp, typename... Ts>
    constexpr decltype(auto) back(Tp&& t, Ts&&... ts) noexcept {
      return [](meta::eat<Ts>..., auto&& x) -> decltype(x) {
        return std::forward<decltype(x)>(x);
      }(std::forward<Tp>(t), std::forward<Ts>(ts)...);
    }
  }
}
}
