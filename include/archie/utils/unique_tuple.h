#ifndef ARCHIE_UTILS_UNIQUE_TUPLE_H_INCLUDED
#define ARCHIE_UTILS_UNIQUE_TUPLE_H_INCLUDED

#include <archie/utils/meta/is_distinct.h>

namespace archie {
namespace utils {

  template <typename... Types>
  struct UniqueTuple : std::tuple<Types...> {
    static_assert(meta::is_distinct<Types...>::value, "");
    using BaseType = std::tuple<Types...>;
    using BaseType::BaseType;
  };
}
}

#endif
