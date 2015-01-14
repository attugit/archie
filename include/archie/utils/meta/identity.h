#ifndef ARCHIE_UTILS_META_IDENTITY_H_INCLUDED
#define ARCHIE_UTILS_META_IDENTITY_H_INCLUDED

#include <archie/utils/meta/returns.h>
#include <archie/utils/meta/apply.h>

namespace archie {
namespace utils {
  namespace meta {

    template <typename Tp>
    using identity = returns<Tp>;

    template <typename Tp>
    using identity_t = apply_t<identity, Tp>;
  }
}
}

#endif
