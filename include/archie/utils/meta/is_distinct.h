#ifndef ARCHIE_UTILS_META_IS_DISTINCT_H_INCLUDED
#define ARCHIE_UTILS_META_IS_DISTINCT_H_INCLUDED

#include <type_traits>
#include <archie/utils/meta/sequence.h>

namespace archie {
namespace utils {
  namespace meta {

    template <typename... Ts>
    using is_distinct = typename sequence<Ts...>::is_distinct;
  }
}
}

#endif
