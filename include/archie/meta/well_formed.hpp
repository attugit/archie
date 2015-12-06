#pragma once

#include <archie/meta/apply.hpp>
#include <archie/meta/ignore.hpp>
#include <archie/meta/returns.hpp>

namespace archie {
namespace meta {
  template <typename...>
  struct well_formed : returns<ignore> {};

  template <typename... Ts>
  using well_formed_t = apply_t<well_formed, Ts...>;
}
}
