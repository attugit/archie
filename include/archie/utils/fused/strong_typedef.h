#pragma once

#include <utility>
#include <type_traits>
#include <archie/utils/meta/returns.h>
#include <archie/utils/meta/requires.h>

namespace archie {
namespace utils {
  namespace fused {
    template <typename...>
    struct strong_typedef;

    template <typename Up, typename... Ts>
    strong_typedef<Ts...>& copy_assing(strong_typedef<Ts...>&, Up&&);

    inline namespace policy {
      template <typename>
      struct eq {};
      template <typename>
      struct lt {};
      template <typename Tp>
      struct add {};

      template <typename Tp, typename... Ps>
      using has = meta::requires_all<std::is_base_of<Ps, Tp>...>;
    }

    template <typename X, typename Tp, typename... policies>
    struct strong_typedef<X, Tp, policies...> : private policies... {
      using self_t = strong_typedef<X, Tp, policies...>;
      using type = X;
      using const_reference = Tp const&;
      template <typename... Us>
      constexpr explicit strong_typedef(Us&&... us)
          : value{std::forward<Us>(us)...} {}

      constexpr explicit operator const_reference() const { return value; }

      Tp const* operator->() const { return &value; }
      Tp* operator->() { return &value; }

      template <typename Up>
      friend type& copy_assing(type& st, Up&& v) {
        st.value = std::forward<Up>(v);
        return st;
      }

      template <typename Up>
      friend void move_assing(type& st, Up&& v) {
        st.value = std::move(v);
      }

      template <typename Up, typename = policy::has<type, eq<Up>>>
      friend bool operator==(type const& t, Up const& u) {
        return static_cast<Tp const&>(t) == static_cast<Tp const&>(u);
      }
      template <typename Up, typename = policy::has<type, eq<Up>>>
      friend bool operator==(Up const& u, type const& s) {
        return (s == u);
      }
      template <typename Up, typename = policy::has<type, eq<Up>>>
      friend bool operator!=(type const& s, Up const& u) {
        return !(s == u);
      }
      template <typename Up, typename = policy::has<type, eq<Up>>>
      friend bool operator!=(Up const& u, type const& s) {
        return s != u;
      }
      template <typename Up, typename = policy::has<type, lt<Up>>>
      friend bool operator<(type const& s, Up const& u) {
        return static_cast<Tp const&>(s) < static_cast<Tp const&>(u);
      }
      template <typename Up, typename = policy::has<type, lt<Up>>>
      friend bool operator<(Up const& u, type const& s) {
        return static_cast<Tp const&>(u) < static_cast<Tp const&>(s);
      }
      template <typename Up, typename = policy::has<type, lt<Up>>>
      friend bool operator>(type const& s, Up const& u) {
        return (u < s);
      }
      template <typename Up, typename = policy::has<type, lt<Up>>>
      friend bool operator>(Up const& u, type const& s) {
        return (s < u);
      }
      template <typename Up, typename = policy::has<type, lt<Up>>>
      friend bool operator<=(type const& s, Up const& u) {
        return !(s > u);
      }
      template <typename Up, typename = policy::has<type, lt<Up>>>
      friend bool operator<=(Up const& u, type const& s) {
        return !(u > s);
      }
      template <typename Up, typename = policy::has<type, lt<Up>>>
      friend bool operator>=(type const& s, Up const& u) {
        return !(s < u);
      }
      template <typename Up, typename = policy::has<type, lt<Up>>>
      friend bool operator>=(Up const& u, type const& s) {
        return !(u < s);
      }
      template <typename Up, typename = policy::has<type, add<Up>>>
      friend type& operator+=(type& t, Up const& u) {
        return copy_assing(t, static_cast<Tp const&>(t) +
                                  static_cast<Tp const&>(u));
      }
      template <typename Up, typename = policy::has<type, add<Up>>>
      friend type operator+(type const& t, Up const& u) {
        type ret = t;
        ret += u;
        return ret;
      }

    private:
      Tp value;
    };
  }
}
}
