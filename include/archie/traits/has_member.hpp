#pragma once

#include <archie/boolean.hpp>
#include <archie/meta/well_formed.hpp>
#include <archie/type_list.hpp>

namespace archie {
namespace traits {
  template <template <typename...> class, typename, typename = meta::well_formed_t<>>
  struct has_member : decltype(fused::False) {
  };

  template <template <typename...> class F, typename... Ts>
  struct has_member<F, meta::type_list<Ts...>, meta::well_formed_t<F<Ts...>>>
      : decltype(fused::True) {
  };
}
}
