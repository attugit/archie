#pragma once

#include <archie/utils/fused/tuple.h>
#include <archie/utils/traits/is_fused_tuple.h>
#include <archie/utils/meta/boolean.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct concat_ {
      private:
        template <typename Vp, typename... Ts, typename... Us>
        constexpr decltype(auto) eval(meta::boolean<false>,
                                      tuple<Ts...> const& t, Vp&& v,
                                      Us&&... us) const {
          return make_tuple(get<Ts>(t)..., std::forward<Vp>(v),
                            std::forward<Us>(us)...);
        }
        template <typename... Ts, typename... Us>
        constexpr decltype(auto) eval(meta::boolean<true>,
                                      tuple<Ts...> const& t,
                                      tuple<Us...> const& u) const {
          return make_tuple(get<Ts>(t)..., get<Us>(u)...);
        }

      public:
        template <typename Tp, typename... Us>
        constexpr decltype(auto) operator()(tuple<Us...> const& us,
                                            Tp&& t) const {
          return eval(traits::is_fused_tuple<Tp>{}, us, std::forward<Tp>(t));
        }
        template <typename V1, typename V2, typename... Ts, typename... Us>
        constexpr decltype(auto) operator()(tuple<Ts...> const& t, V1&& v1,
                                            V2&& v2, Us&&... us) const {
          return eval(meta::boolean<false>{}, t, std::forward<V1>(v1),
                      std::forward<V2>(v2), std::forward<Us>(us)...);
        }
      };
    }
    constexpr detail::concat_ concat{};
  }
}
}
