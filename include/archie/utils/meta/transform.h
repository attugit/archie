#ifndef ARCHIE_UTILS_META_TRANSFORM_H_INCLUDED
#define ARCHIE_UTILS_META_TRANSFORM_H_INCLUDED

#include <archie/utils/meta/sequence.h>

namespace archie {
namespace utils {
  namespace meta {

    template <template <typename> class F, typename... Xs>
    using transform = typename sequence<Xs...>::template transform<F>;

    template <template <typename> class F, typename... Xs>
    using transform_t = typename transform<F, Xs...>::type;
  }
}
}

#endif
