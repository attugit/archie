#pragma once

#include <archie/meta/eval.h>
#include <archie/meta/take.h>
#include <archie/meta/tail.h>
#include <archie/meta/type_list.h>

namespace archie {
namespace meta {
  template <std::size_t i, std::size_t j, typename... Ts>
  struct slice : returns<tail_t<i, take_t<j, Ts...>>> {};

  template <std::size_t i, std::size_t j, typename... Ts>
  struct slice<i, j, type_list<Ts...>> : slice<i, j, Ts...> {};

  template <std::size_t i, std::size_t j, typename... Ts>
  using slice_t = eval<slice<i, j, Ts...>>;
}
}
