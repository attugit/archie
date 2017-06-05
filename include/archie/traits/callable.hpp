#pragma once

#include <utility>

namespace archie
{
  inline namespace models
  {
    struct Callable {
      template <typename F, typename... Ts>
      auto require(F&& f, Ts&&... xs) -> decltype(f(std::forward<Ts>(xs)...));
    };
  }
}
