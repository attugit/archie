#pragma once

#include <utility>
#include <archie/meta/eval.hpp>
#include <archie/meta/returns.hpp>
#include <archie/type_list.hpp>
#include <archie/meta/at.hpp>
#include <archie/meta/indexable.hpp>

namespace archie {
namespace meta {
  namespace detail {
    template <typename... other>
    struct take_ {
    public:
      template <typename... Us>
      using apply = returns<type_list<at_t<other::value, Us...>...>>;
    };
  }
  template <std::size_t n, typename... Ts>
  struct take : indexable_t<detail::take_, n>::template apply<Ts...> {
  };

  template <std::size_t n, typename... Ts>
  struct take<n, type_list<Ts...>> : take<n, Ts...> {
  };

  template <std::size_t n, typename... Ts>
  using take_t = eval<take<n, Ts...>>;
}
}
