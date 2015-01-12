#ifndef ARCHIE_UTILS_META_TRANSFORM_H_INCLUDED
#define ARCHIE_UTILS_META_TRANSFORM_H_INCLUDED

#include <archie/utils/meta/type_list.h>
#include <archie/utils/meta/returns.h>

namespace archie {
namespace utils {
  namespace meta {

    template <template <typename> class F, typename... Xs>
    using transform = returns<type_list<typename F<Xs>::type...>>;

    template <template <typename> class F, typename... Xs>
    using transform_t = typename transform<F, Xs...>::type;
  }
}
}

#endif
