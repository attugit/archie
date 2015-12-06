#pragma once

#include <archie/traits/model_of.h>
#include <archie/traits/callable.h>

namespace archie {
namespace traits {
  template <typename F, typename... Ts>
  using is_callable = model_of<Callable(F, Ts...)>;
}
}
