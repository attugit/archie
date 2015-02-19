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
          return static_cast<Tp const&>(u) == static_cast<Tp const&>(v);
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
      friend void copy_assing(self_t& st, Up&& v) {
        st.value = std::forward<Up>(v);
      }

      template <typename Up>
      friend void move_assing(self_t& st, Up&& v) {
        st.value = std::move(v);
      }

      template <typename Up, typename = meta::requires<std::is_base_of<
                                 policy::equality_comparable<Up>, self_t>>>
      friend bool operator==(self_t const& s, Up const& u) {
        return s.policy::equality_comparable<Up>::eq(s, u);
      }
      template <typename Up, typename = meta::requires<std::is_base_of<
                                 policy::equality_comparable<Up>, self_t>>>
      friend bool operator==(Up const& u, self_t const& s) {
        return (s == u);
      }
      template <typename Up, typename = meta::requires<std::is_base_of<
                                 policy::equality_comparable<Up>, self_t>>>
      friend bool operator!=(self_t const& s, Up const& u) {
        return !(s == u);
      }
      template <typename Up, typename = meta::requires<std::is_base_of<
                                 policy::equality_comparable<Up>, self_t>>>
      friend bool operator!=(Up const& u, self_t const& s) {
        return s != u;
      }

    private:
      Tp value;
    };
  }
}
}
