#pragma once

#include <archie/utils/fused/boolean.h>
#include <archie/utils/meta/well_formed.h>
#include <archie/utils/meta/type_list.h>

namespace archie {
namespace utils {
  namespace traits {

    template <template <typename...> class, typename, typename = meta::well_formed_t<>>
    struct has_member : decltype(fused::False) {};

    template <template <typename...> class F, typename... Ts>
    struct has_member<F, meta::type_list<Ts...>, meta::well_formed_t<F<Ts...>>>
        : decltype(fused::True) {};
  }
}
}
