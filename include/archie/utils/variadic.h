#ifndef ARCHIE_UTILS_VARIADIC_H_INCLUDED
#define ARCHIE_UTILS_VARIADIC_H_INCLUDED

#include <utility>

namespace archie {
namespace utils {

  template <typename Tp>
  using Alias = Tp;

  template <typename Func, typename... Args>
  constexpr void for_each(Func&& func, Args&&... args) {
    (void)Alias<int[]>{(func(std::forward<Args>(args)), 1)...};
  }
}
}

#endif
