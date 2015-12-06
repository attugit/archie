#pragma once

#include <utility>
#include <archie/meta/type_list.h>
#include <archie/meta/take.h>
#include <archie/meta/indexable.h>
#include <archie/meta/static_constexpr_storage.h>
#include <archie/meta/listed.h>
#include <archie/meta/as_type_list.h>
#include <archie/fused/tuple.h>
#include <archie/fused/nth.h>
#include <archie/fused/as_tuple.h>

namespace archie {
namespace fused {
  namespace detail {
    template <std::size_t... ids>
    struct take_ {
      template <typename... Ts>
      struct impl {
        template <typename Up>
        constexpr decltype(auto) operator()(Up&& u) const {
          return fused::tuple<meta::at_t<ids, Ts...>...>(get<ids>(std::forward<Up>(u))...);
        }
      };
      template <typename Tp>
      constexpr decltype(auto) operator()(Tp&& t) const {
        return meta::listed_t<impl, meta::as_type_list_t<std::decay_t<Tp>>>{}(std::forward<Tp>(t));
      }
      template <typename... Ts>
      constexpr decltype(auto) operator()(Ts&&... ts) const {
        return make_tuple(fused::nth<ids>(ts...)...);
      }
    };
    template <std::size_t N>
    using indexed_take_ = meta::indexable_t<detail::take_, N>;
  }

  template <std::size_t N>
  static constexpr auto const& take = meta::instance<detail::indexed_take_<N>>();
}
}
