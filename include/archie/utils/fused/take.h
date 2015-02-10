#pragma once

#include <utility>
#include <archie/utils/meta/type_list.h>
#include <archie/utils/meta/take.h>
#include <archie/utils/meta/indexable.h>
#include <archie/utils/meta/variable_template.h>
#include <archie/utils/fused/tuple.h>
#include <archie/utils/fused/nth.h>
#include <archie/utils/fused/as_tuple.h>
#include <config.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      template <std::size_t... ids>
      struct take {
        template <typename... Ts>
        constexpr decltype(auto) operator()(tuple<Ts...> const& t) const {
          return fused::tuple<meta::at_t<ids, Ts...>...>(get<ids>(t)...);
        }
        template <typename... Ts>
        constexpr decltype(auto) operator()(tuple<Ts...>&& t) const {
          return make_tuple(get<ids>(t)...);
        }
        template <typename... Ts>
        constexpr decltype(auto) operator()(tuple<Ts...>& t) const {
          return make_tuple(get<ids>(t)...);
        }
        template <typename... Ts>
        constexpr decltype(auto) operator()(Ts&&... ts) const {
          return make_tuple(fused::nth<ids>(ts...)...);
        }
      };
    }
#if defined(HAS_VARIABLE_TEMPLATES)
    template <std::size_t n>
    constexpr meta::indexable_t<detail::take, n> take{};
#else
    template <std::size_t n>
    struct take_v
        : meta::variable_template<meta::indexable_t<detail::take, n>> {};

    template <std::size_t n, typename... Ts>
    decltype(auto) take(tuple<Ts...> const& t) {
      return fused::take_v<n>::value(t);
    }
#endif
  }
}
}
