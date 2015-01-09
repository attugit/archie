#ifndef ARCHIE_UTILS_META_APPLY_H_INCLUDED
#define ARCHIE_UTILS_META_APPLY_H_INCLUDED

#include <utility>

namespace archie {
namespace utils {
  namespace meta {
    template <template <typename...> class MetaFunction, typename... Args>
    struct apply {
      using type = typename MetaFunction<Args...>::type;
    };

    template <template <typename...> class MetaFunction, typename... Args>
    using apply_t = typename apply<MetaFunction, Args...>::type;
  }
}
}

#endif
