#pragma once

#include <utility>
#include <archie/utils/fused/tuple.h>
#include <archie/utils/fused/apply.h>
#include <archie/utils/meta/as_type_list.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct tail_ {
      private:
        template <typename>
        struct impl;

        template <typename Rp, typename... Vs>
        struct impl<meta::type_list<Rp, Vs...>> {
          template <typename Tp, typename... Us>
          constexpr decltype(auto) operator()(Tp&&, Us&&... us) const {
            return fused::tuple<Vs...>(std::forward<Us>(us)...);
          }
        };

      public:
        template <typename Tp>
        constexpr decltype(auto) operator()(Tp&& t) const {
          return fused::apply(impl<meta::as_type_list_t<std::decay_t<Tp>>>{},
                              std::forward<Tp>(t));
        }
      };
    }
    constexpr detail::tail_ tail{};
  }
}
}
