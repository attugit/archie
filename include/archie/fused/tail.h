#pragma once

#include <utility>
#include <archie/fused/tuple.h>
#include <archie/fused/apply.h>
#include <archie/meta/listed.h>
#include <archie/meta/ignore.h>
#include <archie/meta/as_type_list.h>
#include <archie/meta/static_constexpr_storage.h>

namespace archie {
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
        return fused::apply(meta::listed_t<impl, meta::as_type_list_t<std::decay_t<Tp>>>{},
                            std::forward<Tp>(t));
      }
    };
  }
  static constexpr auto const& tail = meta::instance<detail::tail_>();
}
}
