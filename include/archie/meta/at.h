#pragma once

#include <utility>
#include <archie/meta/eval.h>
#include <archie/meta/ignore.h>
#include <archie/meta/returns.h>
#include <archie/meta/number.h>
#include <archie/meta/type_list.h>
#include <archie/meta/indexable.h>

namespace archie {
namespace meta {
  namespace detail {
    template <std::size_t... other>
    struct at {
      template <typename Tp>
      static auto skip(eat_n<other>..., Tp&&, ...) noexcept -> returns<Tp>;

      template <typename... Ts>
      using apply = decltype(skip(std::declval<Ts>()...));
    };
  }

  template <std::size_t n>
  using placeholder = indexable_t<detail::at, n>;

  template <std::size_t n, typename... Ts>
  struct at : placeholder<n>::template apply<Ts...> {};

  template <std::size_t n, typename... Ts>
  struct at<n, type_list<Ts...>> : at<n, Ts...> {};

  template <std::size_t n, typename... Ts>
  using at_t = eval<at<n, Ts...>>;
}
}
