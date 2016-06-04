#pragma once

#include <utility>
#include <archie/fused/tuple.hpp>
#include <archie/fused/apply.hpp>
#include <archie/ignore.hpp>
#include <archie/meta/static_constexpr_storage.hpp>

namespace archie::fused
{
  namespace detail
  {
    struct tail_ {
    private:
      template <typename, typename... Ts>
      struct impl {
        template <typename... Us>
        constexpr decltype(auto) operator()(decltype(ignore), Us&&... us) const
        {
          return fused::tuple<Ts...>(std::forward<Us>(us)...);
        }
      };

    public:
      template <template <typename...> typename F, typename... T>
      constexpr decltype(auto) operator()(F<T...> const& t) const
      {
        return fused::apply(impl<T...>{}, std::forward<decltype(t)>(t));
      }
    };
  }
  static constexpr auto const& tail = meta::instance<detail::tail_>();
}
