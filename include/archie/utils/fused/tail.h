#pragma once

#include <utility>
#include <archie/utils/fused/tuple.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct tail_ {
      private:
        template <typename... Vs>
        struct impl {
          template <typename Tp, typename... Us>
          constexpr decltype(auto) operator()(Tp&&, Us&&... us) const {
            return fused::tuple<Vs...>(std::forward<Us>(us)...);
          }
        };

      public:
        template <typename Tp, typename... Us>
        constexpr decltype(auto) operator()(fused::tuple<Tp, Us...>& t) const {
          return t.apply(impl<Us...>{});
        }
        template <typename Tp, typename... Us>
        constexpr decltype(auto) operator()(
            fused::tuple<Tp, Us...> const& t) const {
          return t.apply(impl<Us...>{});
        }
        template <typename Tp, typename... Us>
        constexpr decltype(auto) operator()(fused::tuple<Tp, Us...>&& t) const {
          return t.apply(impl<Us...>{});
        }
      };
    }
    constexpr detail::tail_ tail{};
  }
}
}
