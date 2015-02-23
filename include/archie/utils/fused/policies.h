#pragma once

#include <type_traits>
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
  }
}
}
