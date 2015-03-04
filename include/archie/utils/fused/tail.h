#pragma once

#include <utility>
#include <archie/utils/fused/tuple.h>
#include <archie/utils/fused/apply.h>
#include <archie/utils/meta/listed.h>
#include <archie/utils/meta/ignore.h>
#include <archie/utils/meta/as_type_list.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct tail_ {
      private:
        template <typename, typename... Ts>
        struct impl {
          template <typename... Us>
          constexpr decltype(auto) operator()(meta::ignore, Us&&... us) const {
            return fused::tuple<Ts...>(std::forward<Us>(us)...);
          }
        };

      public:
        template <typename Tp>
        constexpr decltype(auto) operator()(Tp&& t) const {
          return fused::apply(
              meta::listed_t<impl, meta::as_type_list_t<std::decay_t<Tp>>>{},
              std::forward<Tp>(t));
        }
      };
    }
    constexpr auto const tail = detail::tail_{};
  }
}
}
