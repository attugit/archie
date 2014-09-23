#ifndef ARCHIE_UTILS_SELECT_H_INCLUDED
#define ARCHIE_UTILS_SELECT_H_INCLUDED

#include <archie/utils/unique_tuple.h>
#include <tuple>

namespace archie {
namespace utils {

  template <typename... Types>
  using Selection = UniqueTuple<Types&...>;

  template <typename... Args>
  struct Select {
    template <typename... Types>
    static Selection<Args...> from(UniqueTuple<Types...>& ut) {
      return std::tie(std::get<Args>(ut)...);
    }
  };
}
}

#endif
