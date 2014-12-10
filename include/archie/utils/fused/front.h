#ifndef ARCHIE_UTILS_FUSED_FRONT_H_INCLUDED
#define ARCHIE_UTILS_FUSED_FRONT_H_INCLUDED

#include <utility>

namespace archie {
namespace utils {
  namespace fused {

    template <typename Tp, typename... Ts>
    constexpr decltype(auto) front(Tp&& t, Ts&&...) noexcept {
      return std::forward<Tp>(t);
    }
  }
}
}

#endif
