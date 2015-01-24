#pragma once

#include <utility>
#include <archie/utils/meta/ignore.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct for_each {
        template <typename Func, typename... Args>
        decltype(auto) operator()(Func&& func, Args&&... args) const {
          meta::ignore{(func(std::forward<Args>(args)), 0)...};
          return func;
        }
      };
    }

    constexpr detail::for_each for_each{};
  }
}
}
