#pragma once

#include <archie/meta.hpp>
#include <archie/meta/well_formed.hpp>

namespace archie::traits
{
  template <template <typename...> class, typename, typename = meta::well_formed_t<>>
  struct has_member : decltype(fused::False) {
  };

  template <template <typename...> class F, typename... Ts>
  struct has_member<F, meta::type_list<Ts...>, meta::well_formed_t<F<Ts...>>>
      : decltype(fused::True) {
  };
}
