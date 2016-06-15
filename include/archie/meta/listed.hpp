#pragma once

#include <archie/meta.hpp>

namespace archie::meta
{
  template <template <typename...> class, typename>
  struct listed;

  template <template <typename...> class C, typename... Ts>
  struct listed<C, type_list<Ts...>> : returns<C<Ts...>> {
  };

  template <template <typename...> class C, typename Tp>
  using listed_t = typename listed<C, Tp>::type;
}
