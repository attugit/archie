#ifndef ARCHIE_UTILS_TRAITS_IS_TYPE_LIST_H_INCLUDED
#define ARCHIE_UTILS_TRAITS_IS_TYPE_LIST_H_INCLUDED

#include <archie/utils/meta/type_list.h>
#include <archie/utils/meta/boolean.h>

namespace archie {
namespace utils {
  namespace traits {

    template <typename>
    struct is_type_list : meta::boolean<false> {};

    template <typename... Ts>
    struct is_type_list<meta::type_list<Ts...>> : meta::boolean<true> {};
  }
}
}

#endif
