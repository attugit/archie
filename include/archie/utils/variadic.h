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

  template <typename... Tp>
  struct Tuple {
    template <typename... Args>
    static decltype(auto) make_storage(Args&&... args) {
      return [args...](auto&& func) -> decltype(auto) {
        return std::forward<decltype(func)>(func)(args...);
      };
    }
    using Storage = decltype(make_storage(std::declval<Tp>()...));
    Storage storage;

    template <typename... Up>
    explicit Tuple(Up&&... u)
        : storage(make_storage(std::forward<Up>(u)...)) {}

    Tuple() : Tuple(Tp {}...) {}

    std::size_t size() const { return sizeof...(Tp); }
  };
}
}

#endif
