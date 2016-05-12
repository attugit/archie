#pragma once

#include <archie/meta/eval.hpp>
#include <archie/meta/logic.hpp>
#include <archie/meta/append.hpp>
#include <archie/meta/returns.hpp>

namespace archie {
namespace meta {
  template <template <typename> class, typename...>
  struct remove_if;

  template <template <typename> class F, typename Tp>
  struct remove_if<F, type_list<Tp>> : append_if_not<F, type_list<>, Tp> {
  };

  template <template <typename> class F, typename Tp, typename... Us>
  struct remove_if<F, type_list<Tp, Us...>>
      : returns<append_t<eval<remove_if<F, type_list<Tp>>>, eval<remove_if<F, type_list<Us...>>>>> {
  };

  template <template <typename> class F, typename Tp>
  using remove_if_t = eval<remove_if<F, Tp>>;
}
}
