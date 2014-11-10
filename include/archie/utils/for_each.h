#ifndef ARCHIE_UTILS_FOR_EACH_H_INCLUDED
#define ARCHIE_UTILS_FOR_EACH_H_INCLUDED

#include <utility>
#include <archie/utils/alias.h>

namespace archie {
namespace utils {

  template <typename Func, typename... Args>
  constexpr void for_each(Func&& func, Args&&... args) {
    (void)Alias<int[]>{(func(std::forward<Args>(args)), 0)...};
  }
}
}

#endif
