#pragma once

#include <archie/meta/apply.h>
#include <archie/meta/ignore.h>
#include <archie/meta/returns.h>

namespace archie {
namespace meta {
  template <typename...>
  struct well_formed : returns<ignore> {};

  template <typename... Ts>
  using well_formed_t = apply_t<well_formed, Ts...>;
}
}
