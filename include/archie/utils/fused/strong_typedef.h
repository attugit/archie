#pragma once

#include <utility>
#include <archie/utils/meta/returns.h>

namespace archie {
namespace utils {
  namespace fused {

    template <typename Tp>
    struct strong_typedef : meta::returns<Tp> {
      using const_reference = Tp const&;
      template <typename... Us>
      constexpr explicit strong_typedef(Us&&... us)
          : value{std::forward<Us>(us)...} {}

      constexpr explicit operator const_reference() const { return value; }

      Tp const* operator->() const { return &value; }

      Tp* operator->() { return &value; }

      template <typename Up>
      friend void copy_assing(strong_typedef<Tp>& st, Up&& v) {
        st.value = std::forward<Up>(v);
      }

      template <typename Up>
      friend void move_assing(strong_typedef<Tp>& st, Up&& v) {
        st.value = std::move(v);
      }

    private:
      Tp value;
    };
  }
}
}
