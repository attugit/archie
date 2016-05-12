#pragma once

#include <utility>
#include <archie/meta/type_list.hpp>
#include <archie/meta/take.hpp>
#include <archie/meta/indexable.hpp>
#include <archie/meta/static_constexpr_storage.hpp>
#include <archie/meta/listed.hpp>
#include <archie/meta/as_type_list.hpp>
#include <archie/fused/tuple.hpp>
#include <archie/fused/nth.hpp>
#include <archie/fused/as_tuple.hpp>

namespace archie {
namespace fused {
  namespace detail {
    template <std::size_t... ids>
    struct take_ {
      template <typename... Ts>
      struct impl {
        template <typename Up>
        constexpr decltype(auto) operator()(Up&& u) const
        {
          return fused::tuple<meta::at_t<ids, Ts...>...>(get<ids>(std::forward<Up>(u))...);
        }
      };
      template <typename Tp>
      constexpr decltype(auto) operator()(Tp&& t) const
      {
        return meta::listed_t<impl, meta::as_type_list_t<std::decay_t<Tp>>>{}(std::forward<Tp>(t));
      }
      template <typename... Ts>
      constexpr decltype(auto) operator()(Ts&&... ts) const
      {
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
