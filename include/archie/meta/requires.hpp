#pragma once

#include <utility>
#include <archie/meta/logic.hpp>
#include <archie/ignore.hpp>

namespace archie::meta
{
  using enabler = ignore;

  template <typename Condition>
  using require = std::enable_if_t<Condition::value, enabler>;

  template <typename... Conditions>
  using require_all = require<std::integral_constant<bool, all<Conditions...>>>;

  template <typename... Conditions>
  using require_any = require<std::integral_constant<bool, any<Conditions...>>>;

  template <typename... Conditions>
  using require_none = require<std::integral_constant<bool, none<Conditions...>>>;
}
