#pragma once

#include <archie/fused/tuple.h>
#include <archie/meta/index_of.h>
#include <functional>

namespace archie {
namespace fused {
  template <typename Tp>
  struct value_view : std::reference_wrapper<Tp> {
    using base_t = std::reference_wrapper<Tp>;
    using base_t::base_t;
    template <typename Up>
    value_view& operator=(Up&& u) {
      this->get() = u;
      return *this;
    }
  };

  template <typename... Ts>
  using tuple_view = tuple<value_view<Ts>...>;

  namespace detail {
    struct make_view_ {
      template <typename... Ts>
      constexpr decltype(auto) operator()(Ts&... args) const {
        return tuple_view<Ts...>(args...);
      }
    };
  }

  constexpr auto const make_view = detail::make_view_{};

  template <std::size_t... idx, typename... Ts>
  decltype(auto) select(tuple<Ts...>& t) {
    return make_view(get<idx>(t)...);
  }

  template <typename... Ts, typename... Us>
  decltype(auto) select(tuple<Us...>& t) {
    return make_view(get<Ts>(t)...);
  }

  template <typename Tp, typename... Us>
  decltype(auto) get(tuple<value_view<Us>...>& t) {
    constexpr auto idx = meta::index_of<Tp>(meta::type_list<Us...>{});
    return get<idx>(t).get();
  }

  template <typename Tp, typename... Us>
  decltype(auto) get(tuple<value_view<Us>...> const& t) {
    constexpr auto idx = meta::index_of<Tp>(meta::type_list<Us...>{});
    return get<idx>(t).get();
  }
}
}
