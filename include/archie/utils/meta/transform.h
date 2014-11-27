#ifndef ARCHIE_UTILS_META_TRANSFORM_H_INCLUDED
#define ARCHIE_UTILS_META_TRANSFORM_H_INCLUDED

#include <archie/utils/meta/type_list.h>

namespace archie {
namespace utils {
  namespace meta {

    template <template <typename> class F, typename... Xs>
    using transform = typename type_list<Xs...>::template transform<F>;

    template <template <typename> class F, typename... Xs>
    using transform_t = typename transform<F, Xs...>::type;
  }
}
}

#endif
