#pragma once

#include <utility>
#include <archie/meta/eval.h>
#include <archie/meta/returns.h>
#include <archie/meta/type_list.h>
#include <archie/meta/at.h>
#include <archie/meta/indexable.h>

namespace archie {
namespace meta {
  namespace detail {
    template <std::size_t... other>
    struct take {
    public:
      template <typename... Us>
      using apply = returns<type_list<at_t<other, Us...>...>>;
    };
  }
  template <std::size_t n, typename... Ts>
  struct take : indexable_t<detail::take, n>::template apply<Ts...> {};

  template <std::size_t n, typename... Ts>
  struct take<n, type_list<Ts...>> : take<n, Ts...> {};

  template <std::size_t n, typename... Ts>
  using take_t = eval<take<n, Ts...>>;
}
}
