#pragma once

#include <utility>
#include <archie/utils/meta/ignore.h>

namespace archie {
namespace utils {
  namespace fused {

    template <typename Func, typename... Args>
    decltype(auto) for_each(Func&& func, Args&&... args) {
      meta::ignore{(func(std::forward<Args>(args)), 0)...};
      return func;
    }
  }
}
}
