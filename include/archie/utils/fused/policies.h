#pragma once

#include <type_traits>
#include <iterator>
#include <archie/utils/meta/requires.h>
#include <archie/utils/meta/identity.h>

namespace archie {
namespace utils {
  namespace fused {

    template <typename, typename, typename...>
    struct strong_typedef;

    template <typename Up, typename Vp, typename... Ps>
    Vp& extract(strong_typedef<Up, Vp, Ps...>&);
    template <typename Up, typename Vp, typename... Ps>
    Vp const& extract(strong_typedef<Up, Vp, Ps...> const&);
    template <typename Up, typename Vp, typename... Ps>
    Vp&& extract(strong_typedef<Up, Vp, Ps...>&&);

    inline namespace policy {
      template <typename>
      struct eq {};
      template <typename>
      struct lt {};
      template <typename Tp>
      struct add {};
      struct iterate {};

      template <typename Tp, typename... Ps>
      using has = meta::requires_all<std::is_base_of<Ps, Tp>...>;
    }

    template <typename Tp, typename = policy::has<Tp, eq<meta::eval<Tp>>>>
    bool operator==(Tp const& t, meta::identity_t<meta::eval<Tp>> const& u) {
      return fused::extract(t) == u;
    }
    template <typename Tp, typename = policy::has<Tp, eq<meta::eval<Tp>>>>
    bool operator==(meta::identity_t<meta::eval<Tp>> const& u, Tp const& t) {
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
      return fused::extract(t) < static_cast<meta::eval<Tp> const&>(u);
    }
    template <typename Tp, typename Up, typename = policy::has<Tp, lt<Up>>>
    bool operator<(Up const& u, Tp const& t) {
      return static_cast<meta::eval<Tp> const&>(u) < fused::extract(t);
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
      return copy_assing(t, fused::extract(t) +
                                static_cast<meta::eval<Tp> const&>(u));
    }
    template <typename Tp, typename Up, typename = policy::has<Tp, add<Up>>>
    Tp operator+(Tp const& t, Up const& u) {
      Tp ret = t;
      ret += u;
      return ret;
    }
    template <typename Tp,
              typename = policy::has<std::remove_reference_t<Tp>, iterate>>
    decltype(auto) begin(Tp&& t) {
      using std::begin;
      return begin(extract(std::forward<Tp>(t)));
    }
    template <typename Tp,
              typename = policy::has<std::remove_reference_t<Tp>, iterate>>
    decltype(auto) end(Tp&& t) {
      using std::end;
      return end(extract(std::forward<Tp>(t)));
    }
  }
}
}
