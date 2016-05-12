#pragma once

#include <utility>
#include <archie/fused/tuple.hpp>
#include <archie/fused/apply.hpp>
#include <archie/meta/listed.hpp>
#include <archie/meta/ignore.hpp>
#include <archie/meta/as_type_list.hpp>
#include <archie/meta/static_constexpr_storage.hpp>

namespace archie {
namespace fused {
  namespace detail {
    struct tail_ {
    private:
      template <typename, typename... Ts>
      struct impl {
        template <typename... Us>
        constexpr decltype(auto) operator()(meta::ignore, Us&&... us) const
        {
          return fused::tuple<Ts...>(std::forward<Us>(us)...);
        }
      };

    public:
      template <typename Tp>
      constexpr decltype(auto) operator()(Tp&& t) const
      {
        return fused::apply(meta::listed_t<impl, meta::as_type_list_t<std::decay_t<Tp>>>{},
                            std::forward<Tp>(t));
      }
    };
  }
  static constexpr auto const& tail = meta::instance<detail::tail_>();
}
}
