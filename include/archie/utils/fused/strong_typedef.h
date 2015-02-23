#pragma once

#include <utility>
#include <type_traits>
#include <archie/utils/meta/returns.h>
#include <archie/utils/meta/requires.h>

namespace archie {
namespace utils {
  namespace fused {
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

    template <typename Tp, typename Up, typename = policy::has<Tp, eq<Up>>>
    bool operator==(Tp const& t, Up const& u) {
      return static_cast<meta::eval<Tp> const&>(t) ==
             static_cast<meta::eval<Tp> const&>(u);
    }
    template <typename Tp, typename Up, typename = policy::has<Tp, eq<Up>>>
    bool operator==(Up const& u, Tp const& t) {
      return (t == u);
    }
    template <typename Tp, typename Up, typename = policy::has<Tp, eq<Up>>>
    bool operator!=(Tp const& t, Up const& u) {
      return !(t == u);
    }
    template <typename Tp, typename Up, typename = policy::has<Tp, eq<Up>>>
    bool operator!=(Up const& u, Tp const& t) {
      return t != u;
    }
    template <typename Tp, typename Up, typename = policy::has<Tp, lt<Up>>>
    bool operator<(Tp const& t, Up const& u) {
      return static_cast<meta::eval<Tp> const&>(t) <
             static_cast<meta::eval<Tp> const&>(u);
    }
    template <typename Tp, typename Up, typename = policy::has<Tp, lt<Up>>>
    bool operator<(Up const& u, Tp const& t) {
      return static_cast<meta::eval<Tp> const&>(u) <
             static_cast<meta::eval<Tp> const&>(t);
    }
    template <typename Tp, typename Up, typename = policy::has<Tp, lt<Up>>>
    bool operator>(Tp const& t, Up const& u) {
      return (u < t);
    }
    template <typename Tp, typename Up, typename = policy::has<Tp, lt<Up>>>
    bool operator>(Up const& u, Tp const& t) {
      return (t < u);
    }
    template <typename Tp, typename Up, typename = policy::has<Tp, lt<Up>>>
    bool operator<=(Tp const& t, Up const& u) {
      return !(t > u);
    }
    template <typename Tp, typename Up, typename = policy::has<Tp, lt<Up>>>
    bool operator<=(Up const& u, Tp const& t) {
      return !(u > t);
    }
    template <typename Tp, typename Up, typename = policy::has<Tp, lt<Up>>>
    bool operator>=(Tp const& t, Up const& u) {
      return !(t < u);
    }
    template <typename Tp, typename Up, typename = policy::has<Tp, lt<Up>>>
    bool operator>=(Up const& u, Tp const& t) {
      return !(u < t);
    }
    template <typename Tp, typename Up, typename = policy::has<Tp, add<Up>>>
    Tp& operator+=(Tp& t, Up const& u) {
      return copy_assing(t, static_cast<meta::eval<Tp> const&>(t) +
                                static_cast<meta::eval<Tp> const&>(u));
    }
    template <typename Tp, typename Up, typename = policy::has<Tp, add<Up>>>
    Tp operator+(Tp const& t, Up const& u) {
      Tp ret = t;
      ret += u;
      return ret;
    }

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
