#ifndef ARCHIE_UTILS_HAS_MEMBER_H_INCLUDED
#define ARCHIE_UTILS_HAS_MEMBER_H_INCLUDED

#include <archie/utils/meta/boolean.h>
#include <archie/utils/meta/well_formed.h>
#include <archie/utils/meta/type_list.h>

namespace archie {
namespace utils {
  namespace meta {

    template <template <typename...> class, typename,
              typename = meta::well_formed_t<>>
    struct has_member : false_t {};

    template <template <typename...> class F, typename... Ts>
    struct has_member<F, type_list<Ts...>, meta::well_formed_t<F<Ts...>>>
        : true_t {};
  }
}
}

#endif
