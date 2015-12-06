#pragma once

#include <utility>
#include <archie/meta/eval.hpp>
#include <archie/meta/ignore.hpp>
#include <archie/meta/returns.hpp>
#include <archie/meta/number.hpp>
#include <archie/meta/type_list.hpp>
#include <archie/meta/indexable.hpp>

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
