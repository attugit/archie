#pragma once

#include <archie/meta/eval.hpp>
#include <archie/meta/type_list.hpp>
#include <archie/meta/index_of.hpp>
#include <archie/meta/apply.hpp>
#include <archie/meta/boolean.hpp>
#include <archie/meta/returns.hpp>

namespace archie {
namespace meta {
  namespace detail {
    template <typename Tp, typename... Us>
    using find = decltype(index_of<Tp>(type_list<Us...>{}));
  }

  template <typename Tp, typename... Us>
  struct find : detail::find<Tp, Us...> {};

  template <typename Tp, typename... Us>
  struct find<Tp, type_list<Us...>> : find<Tp, Us...> {};

  template <typename Tp, typename... Us>
  using find_t = apply_t<find, Tp, Us...>;

  template <template <typename> class F, typename... Ts>
  struct find_if : find<true_t, boolean<F<Ts>::value>...> {};

  template <template <typename> class F, typename... Ts>
  struct find_if<F, type_list<Ts...>> : find_if<F, Ts...> {};

  template <template <typename> class F, typename... Ts>
  using find_if_t = eval<find_if<F, Ts...>>;
}
}
