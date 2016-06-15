#pragma once

#include <utility>
#include <archie/meta.hpp>
#include <archie/ignore.hpp>

namespace archie::meta
{
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
