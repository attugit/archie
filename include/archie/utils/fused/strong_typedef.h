#pragma once

#include <utility>
#include <archie/utils/meta/returns.h>

namespace archie {
namespace utils {
  namespace fused {
    template <typename X, typename Tp, typename... policies>
    struct strong_typedef : meta::returns<Tp>, private policies... {
      using self_t = strong_typedef<X, Tp, policies...>;
      using const_reference = Tp const&;
      template <typename... Us>
      constexpr explicit strong_typedef(Us&&... us)
          : value{std::forward<Us>(us)...} {}

      constexpr explicit operator const_reference() const { return value; }
      Tp const* operator->() const { return &value; }
      Tp* operator->() { return &value; }

      template <typename Up>
      friend X& copy_assing(X& x, Up&& v) {
        x.value = std::forward<Up>(v);
        return x;
      }
      template <typename Up>
      friend X& move_assing(X& x, Up&& v) {
        x.value = std::move(v);
        return x;
      }

    private:
      Tp value;
    };
  }
}
}
