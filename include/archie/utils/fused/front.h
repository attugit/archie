#pragma once

#include <utility>

namespace archie {
namespace utils {
  namespace fused {

    template <typename Tp>
    constexpr decltype(auto) front(Tp&& t, ...) noexcept {
      return std::forward<Tp>(t);
    }
  }
}
}
