#pragma once

#include <archie/meta/eval.hpp>
#include <archie/type_list.hpp>
#include <archie/meta/returns.hpp>
#include <archie/meta/functional.hpp>
#include <archie/meta/logic.hpp>

namespace archie::meta
{
  template <typename, typename>
  struct append;

  template <typename... Ts, typename... Us>
  struct append<type_list<Ts...>, type_list<Us...>> : returns<type_list<Ts..., Us...>> {
  };

  template <typename Tp, typename... Us>
  struct append<type_list<Us...>, Tp> : returns<type_list<Us..., Tp>> {
  };

  template <typename Tp, typename Up>
  using append_t = eval<append<Tp, Up>>;

  template <template <typename> class, typename...>
  struct append_if;

  template <template <typename> class F, typename Tp, typename... Us>
  struct append_if<F, type_list<Us...>, Tp>
      : returns<if_t<eval<F<Tp>>, append_t<type_list<Us...>, Tp>, type_list<Us...>>> {
  };

  template <template <typename> class F, typename... Ts>
  using append_if_t = eval<append_if<F, Ts...>>;

  template <template <typename> class, typename...>
  struct append_if_not;

  template <template <typename> class F, typename Tp, typename... Us>
  struct append_if_not<F, type_list<Us...>, Tp>
      : returns<if_t<no<eval<F<Tp>>>, append_t<type_list<Us...>, Tp>, type_list<Us...>>> {
  };

  template <template <typename> class F, typename... Ts>
  using append_if_not_t = eval<append_if_not<F, Ts...>>;
}
