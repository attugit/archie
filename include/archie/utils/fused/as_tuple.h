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

    template <typename>
    struct as_type_list;

    template <typename... Ts>
    struct as_type_list<tuple<Ts...>> : meta::returns<meta::type_list<Ts...>> {
      constexpr static decltype(auto) make() {
        return meta::type_list<Ts...>{};
      }
    };

    template <typename Tp>
    using as_type_list_t = typename as_type_list<Tp>::type;
  }
}
}
