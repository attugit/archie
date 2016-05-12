#pragma once

#include <archie/meta/type_list.hpp>
#include <archie/fused/boolean.hpp>

namespace archie {
namespace traits {
  template <typename>
  struct is_type_list : decltype(fused::False) {
  };

  template <typename... Ts>
  struct is_type_list<meta::type_list<Ts...>> : decltype(fused::True) {
  };
}
}
