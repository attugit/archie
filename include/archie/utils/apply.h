#ifndef ARCHIE_UTILS_APPLY_H_INCLUDED
#define ARCHIE_UTILS_APPLY_H_INCLUDED

#include <utility>

namespace archie {
namespace utils {
  template <template <typename...> class MetaFunction, typename... Args>
  using Apply = typename MetaFunction<Args...>::type;

  template <typename Func, typename... Ts>
  constexpr decltype(auto) apply(Func&& func, Ts&&... ts) noexcept(
      noexcept(std::declval<Func>()(std::declval<Ts>()...))) {
    return std::forward<Func>(func)(std::forward<Ts>(ts)...);
  }
}
}

#endif
