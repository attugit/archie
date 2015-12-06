#pragma once

#include <utility>
#include <archie/meta/eval.hpp>
#include <archie/meta/returns.hpp>
#include <archie/meta/type_list.hpp>
#include <archie/meta/ignore.hpp>
#include <archie/meta/number.hpp>
#include <archie/meta/indexable.hpp>

namespace archie {
namespace meta {
  namespace detail {
    template <std::size_t... other>
    struct tail {
      template <typename... Ts>
      static constexpr decltype(auto) slice(eat_n<other>..., Ts...) {
        return type_list<Ts...>{};
      }

      template <typename... Ts>
      using apply = returns<decltype(slice(std::declval<Ts>()...))>;
    };
  }

  template <std::size_t n, typename... Ts>
  struct tail : indexable_t<detail::tail, n>::template apply<Ts...> {};

  template <std::size_t n, typename... Ts>
  struct tail<n, type_list<Ts...>> : tail<n, Ts...> {};

  template <std::size_t n, typename... Ts>
  using tail_t = eval<tail<n, Ts...>>;
}
}
