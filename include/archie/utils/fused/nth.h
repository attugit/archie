#ifndef ARCHIE_UTILS_FUSED_NTH_H_INCLUDED
#define ARCHIE_UTILS_FUSED_NTH_H_INCLUDED

#include <archie/utils/meta/at.h>

namespace archie {
namespace utils {
  namespace fused {
    template <std::size_t n, typename... Ts>
    decltype(auto) nth(Ts&&... args) {
      return meta::at<n>{}(std::forward<Ts>(args)...);
    }
  }
}
}

#endif
