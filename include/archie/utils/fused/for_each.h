#pragma once

#include <utility>
#include <archie/utils/meta/ignore.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct for_each_ {
        template <typename Func, typename... Args>
        decltype(auto) operator()(Func&& func, Args&&... args) const {
          meta::ignore{
              (std::forward<Func>(func)(std::forward<Args>(args)), 0)...};
          return std::forward<Func>(func);
        }
      };
    }
    constexpr detail::for_each_ for_each{};
  }
}
}
