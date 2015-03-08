#pragma once

#include <utility>

namespace archie {
namespace utils {
  inline namespace models {
    struct Callable {
      template <typename F, typename... Ts>
      auto requires(F&& f, Ts&&... xs) -> decltype(f(std::forward<Ts>(xs)...));
    };
  }
}
}
