#ifndef ARCHIE_UTILS_META_IDENTITY_H_INCLUDED
#define ARCHIE_UTILS_META_IDENTITY_H_INCLUDED

#include <archie/utils/meta/returns.h>

namespace archie {
namespace utils {
  namespace meta {

    template <typename Tp>
    using identity = returns<Tp>;

    template <typename Tp>
    using identity_t = typename identity<Tp>::type;
  }
}
}

#endif
