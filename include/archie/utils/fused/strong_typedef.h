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
      struct eq {
        template <typename Up, typename Vp>
        bool eval(Up const& u, Vp const& v) const {
          return static_cast<Tp const&>(u) == static_cast<Tp const&>(v);
        }
      };
      template <typename Tp>
      struct lt {
        template <typename Up, typename Vp>
        bool eval(Up const& u, Vp const& v) const {
          return static_cast<Tp const&>(u) < static_cast<Tp const&>(v);
        }
      };

      template <typename Tp, typename... Ps>
      using has = meta::requires_all<std::is_base_of<Ps, Tp>...>;
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

      template <typename Up, typename = policy::has<self_t, eq<Up>>>
      friend bool operator==(self_t const& s, Up const& u) {
        return s.policy::eq<Up>::eval(s, u);
      }
      template <typename Up, typename = policy::has<self_t, eq<Up>>>
      friend bool operator==(Up const& u, self_t const& s) {
        return (s == u);
      }
      template <typename Up, typename = policy::has<self_t, eq<Up>>>
      friend bool operator!=(self_t const& s, Up const& u) {
        return !(s == u);
      }
      template <typename Up, typename = policy::has<self_t, eq<Up>>>
      friend bool operator!=(Up const& u, self_t const& s) {
        return s != u;
      }
      template <typename Up, typename = policy::has<self_t, lt<Up>>>
      friend bool operator<(self_t const& s, Up const& u) {
        return s.policy::lt<Up>::eval(s, u);
      }
      template <typename Up, typename = policy::has<self_t, lt<Up>>>
      friend bool operator<(Up const& u, self_t const& s) {
        return s.policy::lt<Up>::eval(u, s);
      }
      template <typename Up, typename = policy::has<self_t, lt<Up>>>
      friend bool operator>(Up const& u, self_t const& s) {
        return (s < u);
      }
      template <typename Up, typename = policy::has<self_t, eq<Up>, lt<Up>>>
      friend bool operator<=(self_t const& s, Up const& u) {
        return (s < u) || (s == u);
      }

    private:
      Tp value;
    };
  }
}
}
