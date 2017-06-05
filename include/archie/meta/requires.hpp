#pragma once

#include <utility>
#include <archie/meta/eval.hpp>
#include <archie/meta/logic.hpp>
#include <archie/ignore.hpp>
#include <archie/meta/returns.hpp>

namespace archie::meta
{
  using enabler = ignore;

  template <typename Condition>
  using require = eval<std::enable_if<Condition::value, enabler>>;

  template <typename... Conditions>
  using require_all = require<all<Conditions...>>;

  template <typename... Conditions>
  using require_any = require<any<Conditions...>>;

  template <typename... Conditions>
  using require_none = require<none<Conditions...>>;
}
