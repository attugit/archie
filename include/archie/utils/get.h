#ifndef ARCHIE_UTILS_GET_H_INCLUDED
#define ARCHIE_UTILS_GET_H_INCLUDED

#include <archie/utils/typeset.h>

namespace archie {
namespace utils {

  template <typename T, typename... Types>
  const T& get(TypeSet<Types...> const& ts) {
    return ts.get<T>();
  }

  template <typename T, typename... Types>
  T& get(TypeSet<Types...>& ts) {
    return ts.get<T>();
  }
}
}

#endif
