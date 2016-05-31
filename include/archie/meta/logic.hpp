#pragma once

#include <type_traits>
#include <archie/meta/eval.hpp>
#include <archie/fused/boolean.hpp>
#include <archie/meta/returns.hpp>

namespace archie {
namespace meta {
  template <bool B>
  using negation = boolean<!B>;

  template <bool B>
  using negation_t = eval<negation<B>>;

  template <typename Tp>
  using opposite = negation<Tp::value>;

  template <typename Tp>
  using opposite_t = negation_t<Tp::value>;

  template <typename... Ts>
  struct all : boolean<(... && Ts::value)> {
  };

  template <typename... Ts>
  struct any : boolean<(... || Ts::value)> {
  };

  template <typename... Ts>
  using none = boolean<!any<Ts...>::value>;
}
}
