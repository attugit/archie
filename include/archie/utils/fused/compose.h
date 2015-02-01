#pragma once

#include <utility>
#include <archie/utils/fused/apply.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct compose_ {
        template <typename F, typename G, typename... Ts>
        constexpr decltype(auto) operator()(F&& f, G&& g, Ts&&... args) const {
          return fused::apply(
              std::forward<F>(f),
              fused::apply(std::forward<G>(g), std::forward<Ts>(args)...));
        }
      };
    }
    constexpr detail::compose_ compose{};
  }
}
}
