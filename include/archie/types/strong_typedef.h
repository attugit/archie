#pragma once

#include <utility>
#include <archie/meta/returns.h>

namespace archie {
namespace types {
  template <typename X, typename Tp, typename... policies>
  struct strong_typedef : meta::returns<Tp>, private policies... {
    using self_t = strong_typedef<X, Tp, policies...>;

    template <typename Up, typename Vp, typename... Ps>
    friend Vp& extract(strong_typedef<Up, Vp, Ps...>&);
    template <typename Up, typename Vp, typename... Ps>
    friend Vp const& extract(strong_typedef<Up, Vp, Ps...> const&);
    template <typename Up, typename Vp, typename... Ps>
    friend Vp&& extract(strong_typedef<Up, Vp, Ps...>&&);

    template <typename... Us>
    constexpr explicit strong_typedef(Us&&... us)
        : value{std::forward<Us>(us)...} {}

    constexpr explicit operator Tp const&() const { return value; }
    Tp const* operator->() const { return &extract(*this); }
    Tp* operator->() { return &extract(*this); }

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
  template <typename Up, typename Vp, typename... Ps>
  Vp& extract(strong_typedef<Up, Vp, Ps...>& st) {
    return st.value;
  }
  template <typename Up, typename Vp, typename... Ps>
  Vp const& extract(strong_typedef<Up, Vp, Ps...> const& st) {
    return st.value;
  }
  template <typename Up, typename Vp, typename... Ps>
  Vp&& extract(strong_typedef<Up, Vp, Ps...>&& st) {
    return std::move(st.value);
  }
}
}
