#pragma once

#include <archie/utils/fused/tuple.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct zip {
        template <typename... Ts, typename... Us>
        constexpr tuple<tuple<Ts, Us>...> operator()(
            tuple<Ts...> const& a, tuple<Us...> const& b) const {
          return make_tuple(make_tuple(get<Ts>(a), get<Us>(b))...);
        }
      };
    }
    constexpr detail::zip zip{};
  }
}
}
