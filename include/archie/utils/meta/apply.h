#pragma once

#include <archie/utils/meta/eval.h>
#include <archie/utils/meta/type_list.h>
#include <archie/utils/meta/returns.h>

namespace archie {
namespace utils {
  namespace meta {
    template <template <typename...> class F, typename... Args>
    struct apply : returns<eval<F<Args...>>> {};

    template <template <typename...> class F, typename... Args>
    struct apply<F, type_list<Args...>> : apply<F, Args...> {};

    template <template <typename...> class F, typename... Args>
    using apply_t = eval<apply<F, Args...>>;
  }
}
}
