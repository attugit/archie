#ifndef ARCHIE_UTILS_TYPE_SET_H_INCLUDED
#define ARCHIE_UTILS_TYPE_SET_H_INCLUDED

#include <archie/utils/unique_tuple.h>

namespace archie {
namespace utils {

  template <typename... Types>
  using TypeSet = UniqueTuple<Types...>;
}
}

#endif
