#ifndef ARCHIE_UTILS_FUSED_FRONT_H_INCLUDED
#define ARCHIE_UTILS_FUSED_FRONT_H_INCLUDED

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

#endif
