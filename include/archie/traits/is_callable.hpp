#pragma once

#include <archie/traits/model_of.hpp>
#include <archie/traits/callable.hpp>

namespace archie {
namespace traits {
  template <typename F, typename... Ts>
  using is_callable = model_of<Callable(F, Ts...)>;
}
}
