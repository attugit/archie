#ifndef ARCHIE_UTILS_META_RETURNS_H_INCLUDED
#define ARCHIE_UTILS_META_RETURNS_H_INCLUDED

#include <archie/utils/meta/type_holder.h>

namespace archie {
namespace utils {
  namespace meta {

    template <typename Tp>
    using returns = type_holder<Tp>;

    template <typename Tp>
    using eval = typename Tp::type;
  }
}
}

#endif
