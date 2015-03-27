#pragma once

#include <archie/utils/traits/model_of.h>
#include <archie/utils/traits/callable.h>

namespace archie {
namespace utils {
  namespace traits {
    template <typename F, typename... Ts>
    using is_callable = model_of<Callable(F, Ts...)>;
  }
}
}
