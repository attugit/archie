#pragma once

#include <archie/boolean.hpp>

namespace archie::meta
{
  template <typename... Ts>
  struct all : boolean<(... && Ts::value)> {
  };

  template <typename... Ts>
  struct any : boolean<(... || Ts::value)> {
  };

  template <typename... Ts>
  using none = boolean<!any<Ts...>::value>;

  template <typename T>
  using no = none<T>;
}
