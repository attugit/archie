#pragma once

#include <utility>
#include <archie/fused/tuple.hpp>
#include <archie/fused/apply.hpp>
#include <archie/fused/tail.hpp>
#include <archie/fused/boolean.hpp>
#include <archie/fused/conditional.hpp>
#include <archie/fused/static_if.hpp>
#include <archie/traits/is_fused_tuple.hpp>
#include <archie/traits/is_callable.hpp>

namespace archie {
namespace fused {
  namespace detail {
    struct compose_args_ {
      template <typename F,
                typename... Ts,
                typename = meta::requires<traits::is_callable<F, Ts&&...>>>
      constexpr decltype(auto) operator()(F&& f, Ts&&... ts) const {
        return fused::apply(std::forward<F>(f), std::forward<Ts>(ts)...);
      }
    };

    struct compose_tuple_ {
      template <typename F,
                typename... Ts,
                typename = meta::requires<traits::is_fused_tuple<std::remove_reference_t<F>>>>
      constexpr decltype(auto) operator()(F&& f, Ts&&... ts) const {
        return fused::static_if(meta::boolean<(tuple_size(type_tag<F>{}) == 1)>{})(
            [](auto&& t, auto&&... args) -> decltype(auto) {
              return fused::apply(fused::get<0>(std::forward<decltype(t)>(t)),
                                  std::forward<decltype(args)>(args)...);
            },
            [this](auto&& t, auto&&... args) -> decltype(auto) {
              return fused::apply(fused::get<0>(std::forward<decltype(t)>(t)),
                                  this->operator()(fused::tail(std::forward<decltype(t)>(t)),
                                                   std::forward<decltype(args)>(args)...));
            })(std::forward<F>(f), std::forward<Ts>(ts)...);
      }
    };
  }

  constexpr auto const compose =
      fused::make_conditional(detail::compose_tuple_{}, detail::compose_args_{});
}
}
