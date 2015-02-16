#pragma once

#include <archie/utils/meta/returns.h>
#include <archie/utils/meta/type_list.h>
#include <archie/utils/fused/tuple.h>

namespace archie {
namespace utils {
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
}
