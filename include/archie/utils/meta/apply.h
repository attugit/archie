#ifndef ARCHIE_UTILS_META_APPLY_H_INCLUDED
#define ARCHIE_UTILS_META_APPLY_H_INCLUDED

#include <archie/utils/meta/type_list.h>
#include <archie/utils/meta/returns.h>

namespace archie {
namespace utils {
  namespace meta {
    template <template <typename...> class F, typename... Args>
    struct apply : returns<typename F<Args...>::type> {};

    template <template <typename...> class F, typename... Args>
    struct apply<F, type_list<Args...>> : apply<F, Args...> {};

    template <template <typename...> class F, typename... Args>
    using apply_t = typename apply<F, Args...>::type;
  }
}
}

#endif
