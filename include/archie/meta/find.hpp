#pragma once

#include <archie/meta/base.hpp>
#include <archie/index_of.hpp>

namespace archie::meta
{
  namespace detail
  {
    template <typename Tp, typename... Us>
    using find = decltype(fused::index_of<Tp>(fused::type_list<Us...>));
  }

  template <typename Tp, typename... Us>
  struct find : detail::find<Tp, Us...> {
  };

  template <typename Tp, typename... Us>
  struct find<Tp, type_list<Us...>> : find<Tp, Us...> {
  };

  template <typename Tp, typename... Us>
  using find_t = eval<find<Tp, Us...>>;

  template <template <typename> class F, typename... Ts>
  struct find_if : find<true_t, boolean<F<Ts>::value>...> {
  };

  template <template <typename> class F, typename... Ts>
  struct find_if<F, type_list<Ts...>> : find_if<F, Ts...> {
  };

  template <template <typename> class F, typename... Ts>
  using find_if_t = eval<find_if<F, Ts...>>;
}
