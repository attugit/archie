#ifndef ARCHIE_UTILS_FUSED_FOR_EACH_H_INCLUDED
#define ARCHIE_UTILS_FUSED_FOR_EACH_H_INCLUDED

#include <utility>
#include <archie/utils/meta/alias.h>

namespace archie {
namespace utils {
  namespace fused {

    template <typename Func, typename... Args>
    decltype(auto) for_each(Func&& func, Args&&... args) {
      (void)meta::alias<int[]>{0, (func(std::forward<Args>(args)), 0)...};
      // return std::forward<Func>(func);
      return func;
    }
  }
}
}

#endif
