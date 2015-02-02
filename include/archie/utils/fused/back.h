#pragma once

#include <utility>
#include <archie/utils/meta/ignore.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct back_ {
        template <typename Tp, typename... Ts>
        constexpr decltype(auto) operator()(Tp&& t, Ts&&... ts) const noexcept {
          return [](meta::eat<Ts>..., auto&& x) -> decltype(x) {
            return std::forward<decltype(x)>(x);
          }(std::forward<Tp>(t), std::forward<Ts>(ts)...);
        }
      };
    }
    constexpr detail::back_ back{};
  }
}
}
