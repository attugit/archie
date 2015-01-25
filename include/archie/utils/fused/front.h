#pragma once

#include <utility>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct front {
        template <typename Tp, typename... Us>
        constexpr decltype(auto) operator()(Tp&& t, Us&&...) const noexcept {
          return std::forward<Tp>(t);
        }
      };
    }
    constexpr detail::front front{};
  }
}
}
