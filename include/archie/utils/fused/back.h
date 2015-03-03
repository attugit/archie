#pragma once

#include <utility>
#include <archie/utils/meta/ignore.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct back_ {
        template <typename Tp, typename... Us>
        constexpr decltype(auto) operator()(Tp&& t, Us&&... us) const noexcept {
          return [](meta::eat<Us>..., auto&& x) -> decltype(x) {
            return std::forward<decltype(x)>(x);
          }(std::forward<Tp>(t), std::forward<Us>(us)...);
        }
      };
    }
    constexpr auto const back = detail::back_{};
  }
}
}
