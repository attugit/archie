#pragma once

#include <archie/utils/fused/tuple.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct zip_ {
        template <typename... Ts, typename... Us>
        constexpr decltype(auto) operator()(tuple<Ts...> const& a,
                                            tuple<Us...> const& b) const {
          return make_tuple(make_tuple(get<Ts>(a), get<Us>(b))...);
        }
      };

      struct zip_view_ {
        template <typename... Ts, typename... Us>
        constexpr decltype(auto) operator()(tuple<Ts...>& a,
                                            tuple<Us...>& b) const {
          return make_tuple(tie(get<Ts>(a), get<Us>(b))...);
        }
      };
    }
    constexpr detail::zip_ zip{};
    constexpr detail::zip_view_ zip_view{};
  }
}
}
