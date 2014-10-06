#ifndef ARCHIE_UTILS_APPLY_H_INCLUDED
#define ARCHIE_UTILS_APPLY_H_INCLUDED

#include <archie/utils/unique_tuple.h>
#include <archie/utils/get.h>

namespace archie {
namespace utils {
  template <typename... Args>
  struct Apply {
    template <typename Func, typename... Types>
    static auto call(Func func, UniqueTuple<Types...> const& ut) {
      return func(archie::utils::get<Args>(ut)...);
    }

    template <typename Func, typename... Types>
    static auto call(Func func, UniqueTuple<Types...>& ut) {
      return func(archie::utils::get<Args>(ut)...);
    }
  };
}
}

#endif
