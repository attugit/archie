#pragma once

#include <archie/meta.hpp>
#include <archie/ignore.hpp>

namespace archie::meta
{
  template <typename...>
  struct well_formed : returns<ignore> {
  };

  template <typename... Ts>
  using well_formed_t = eval<well_formed<Ts...>>;
}
