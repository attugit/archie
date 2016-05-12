#pragma once

#include <archie/meta/returns.hpp>
#include <archie/meta/type_list.hpp>

namespace archie {
namespace meta {
  template <template <typename...> class, typename>
  struct listed;

  template <template <typename...> class C, typename... Ts>
  struct listed<C, type_list<Ts...>> : returns<C<Ts...>> {
  };

  template <template <typename...> class C, typename Tp>
  using listed_t = typename listed<C, Tp>::type;
}
}
