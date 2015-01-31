#pragma once

#include <archie/utils/fused/tuple.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct concat {
        template <typename... Ts, typename... Us>
        constexpr decltype(auto) operator()(tuple<Ts...> const& t,
                                            Us&&... us) const {
          return make_tuple(get<Ts>(t)..., std::forward<Us>(us)...);
        }
      };
    }
    constexpr detail::concat concat{};
  }
}
}
