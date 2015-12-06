#pragma once

#include <utility>
#include <archie/meta/eval.h>
#include <archie/meta/logic.h>
#include <archie/meta/ignore.h>
#include <archie/meta/returns.h>

namespace archie {
namespace meta {
  using enabler = ignore;

  template <typename Condition>
  using requires = eval<std::enable_if<Condition::value, enabler>>;

  template <typename... Conditions>
  using requires_all = requires<all<Conditions...>>;

  template <typename... Conditions>
  using requires_any = requires<any<Conditions...>>;

  template <typename... Conditions>
  using requires_none = requires<none<Conditions...>>;
}
}
