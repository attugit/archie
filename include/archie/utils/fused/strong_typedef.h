#pragma once

#include <utility>
#include <type_traits>
#include <archie/utils/meta/returns.h>
#include <archie/utils/meta/requires.h>

namespace archie {
namespace utils {
  namespace fused {
    inline namespace policy {
      template <typename Tp>
      struct equality_comparable {
        template <typename Up, typename Vp>
        bool eq(Up const& u, Vp const& v) const {
          return static_cast<Tp>(u) == static_cast<Vp>(v);
        }
      };
    }

    template <typename Tp, typename... policies>
    struct strong_typedef : meta::returns<Tp>, policies... {
      using self_t = strong_typedef<Tp, policies...>;
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

      template <typename Up, typename = meta::requires<std::is_base_of<
                                 policy::equality_comparable<Up>, self_t>>>
      bool operator==(Up const& u) const {
        return policy::equality_comparable<Up>::eq(*this, u);
      }

    private:
      Tp value;
    };
  }
}
}
