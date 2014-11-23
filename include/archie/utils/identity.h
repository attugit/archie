#ifndef ARCHIE_UTILS_META_IDENTITY_H_INCLUDED
#define ARCHIE_UTILS_META_IDENTITY_H_INCLUDED

#include <archie/utils/type_holder.h>

namespace archie {
namespace utils {
  namespace meta {

    template <typename Tp>
    using identity = type_holder<Tp>;

    template <typename Tp>
    using identity_t = typename identity<Tp>::type;
  }
}
}

#endif
