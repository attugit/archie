#pragma once

#include <archie/meta/returns.hpp>
#include <archie/type_list.hpp>
#include <archie/fused/tuple.hpp>

namespace archie {
namespace fused {
  template <typename... Ts>
  struct as_tuple : meta::returns<tuple<Ts...>> {
    template <typename... Us>
    static decltype(auto) make(Us&&... us)
    {
      return tuple<Ts...>{std::forward<Us>(us)...};
    }
  };

  template <typename... Ts>
  struct as_tuple<meta::type_list<Ts...>> : as_tuple<Ts...> {
  };
}
}
