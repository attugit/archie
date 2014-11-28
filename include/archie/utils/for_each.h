#ifndef ARCHIE_UTILS_FOR_EACH_H_INCLUDED
#define ARCHIE_UTILS_FOR_EACH_H_INCLUDED

#include <utility>
#include <archie/utils/alias.h>

namespace archie {
namespace utils {

  template <typename Func, typename... Args>
  decltype(auto) for_each(Func&& func, Args&&... args) {
    (void)Alias<int[]>{0, (func(std::forward<Args>(args)), 0)...};
    return std::forward<Func>(func);
  }
}
}

#endif
