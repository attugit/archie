#pragma once

#include <utility>
#include <archie/utils/meta/type_list.h>
#include <archie/utils/meta/take.h>
#include <archie/utils/meta/indexable.h>
#include <archie/utils/meta/variable_template.h>
#include <archie/utils/meta/listed.h>
#include <archie/utils/meta/as_type_list.h>
#include <archie/utils/fused/tuple.h>
#include <archie/utils/fused/nth.h>
#include <archie/utils/fused/as_tuple.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      template <std::size_t... ids>
      struct take {
        template <typename... Ts>
        struct impl {
          template <typename Up>
          constexpr decltype(auto) operator()(Up&& u) const {
            return fused::tuple<meta::at_t<ids, Ts...>...>(get<ids>(std::forward<Up>(u))...);
          }
        };
        template <typename Tp>
        constexpr decltype(auto) operator()(Tp&& t) const {
          return meta::listed_t<impl, meta::as_type_list_t<std::decay_t<Tp>>>{}(
              std::forward<Tp>(t));
        }
        template <typename... Ts>
        constexpr decltype(auto) operator()(Ts&&... ts) const {
          return make_tuple(VARTEMPL(fused::nth, ids)(ts...)...);
        }
      };
    }
#if defined(HAS_VARIABLE_TEMPLATES)
    template <std::size_t n>
    constexpr auto const take = meta::indexable_t<detail::take, n>{};
#else
    template <std::size_t n>
    struct take_v : meta::variable_template<meta::indexable_t<detail::take, n>> {};

    template <std::size_t n, typename... Ts>
    decltype(auto) take(tuple<Ts...> const& t) {
      return fused::take_v<n>::value(t);
    }
#endif
  }
}
}
