#ifndef ARCHIE_UTILS_META_BOOLEAN_H_INCLUDED
#define ARCHIE_UTILS_META_BOOLEAN_H_INCLUDED

#include <utility>

namespace archie {
namespace utils {
  namespace meta {

    template <bool B>
    using boolean = std::integral_constant<bool, B>;

    using true_t = boolean<true>;
    using false_t = boolean<false>;
  }
}
}

#endif
