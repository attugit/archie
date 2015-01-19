#pragma once

#include <utility>

namespace archie {
namespace utils {
  namespace fused {

    template <typename Tp, typename... Us>
    constexpr decltype(auto) front(Tp&& t, Us&&...) noexcept {
      return std::forward<Tp>(t);
    }
  }
}
}
