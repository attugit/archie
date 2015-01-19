#pragma once

#include <utility>
#include <archie/utils/meta/type_list.h>
#include <archie/utils/meta/take.h>
#include <archie/utils/fused/tuple.h>
#include <archie/utils/fused/as_tuple.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      template <std::size_t n, typename = std::make_index_sequence<n>>
      struct take;

      template <std::size_t n, std::size_t... ids>
      struct take<n, std::index_sequence<ids...>> {
        template <typename... Ts>
        decltype(auto) operator()(tuple<Ts...> const& t) const {
          return as_tuple<meta::take_t<n, Ts...>>::make(get<ids>(t)...);
        }
      };
    }

    template <std::size_t n, typename... Ts>
    decltype(auto) take(tuple<Ts...> const& t) {
      return detail::take<n>()(t);
    }
  }
}
}
