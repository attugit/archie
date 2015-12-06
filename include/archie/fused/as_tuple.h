#pragma once

#include <archie/meta/returns.h>
#include <archie/meta/type_list.h>
#include <archie/fused/tuple.h>

namespace archie {
namespace fused {
  template <typename... Ts>
  struct as_tuple : meta::returns<tuple<Ts...>> {
    template <typename... Us>
    static decltype(auto) make(Us&&... us) {
      return tuple<Ts...>{std::forward<Us>(us)...};
    }
  };

  template <typename... Ts>
  struct as_tuple<meta::type_list<Ts...>> : as_tuple<Ts...> {};
}
}
