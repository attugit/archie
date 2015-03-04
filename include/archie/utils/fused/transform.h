#pragma once

#include <archie/utils/fused/tuple.h>
#include <archie/utils/fused/apply.h>
#include <utility>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct transform_ {
        template <typename F, typename... Ts>
        decltype(auto) operator()(F&& f, Ts&&... x) const {
          auto make = [g = std::forward<F>(f)](auto&&... xs) {
            return make_tuple(g(std::forward<decltype(xs)>(xs))...);
          };
          return fused::apply(make, std::forward<Ts>(x)...);
        }
      };
    }
    constexpr auto const transform = detail::transform_{};
  }
}
}
