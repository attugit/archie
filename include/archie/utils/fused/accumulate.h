#pragma once

#include <utility>
#include <archie/utils/fused/fold.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct accumulate_ {
        template <typename Tp, typename Up, typename... Vs>
        constexpr decltype(auto) operator()(Tp&& t, Up&& u, Vs&&... vs) const {
          return fused::fold([](auto&& a, auto&& b) {
                               return std::forward<decltype(a)>(a) + std::forward<decltype(b)>(b);
                             },
                             std::forward<Tp>(t),
                             std::forward<Up>(u),
                             std::forward<Vs>(vs)...);
        }
      };
    }
    constexpr auto const accumulate = detail::accumulate_{};
  }
}
}
