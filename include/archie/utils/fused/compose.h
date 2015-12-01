#pragma once

#include <utility>
#include <archie/utils/fused/tuple.h>
#include <archie/utils/fused/apply.h>
#include <archie/utils/fused/tail.h>
#include <archie/utils/fused/boolean.h>
#include <archie/utils/fused/conditional.h>
#include <archie/utils/traits/is_fused_tuple.h>
#include <archie/utils/traits/is_callable.h>

namespace archie {
namespace utils {
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
          return impl(meta::boolean<(tuple_size(type_tag<F>{}) == 1)>{}, std::forward<F>(f),
                      std::forward<Ts>(ts)...);
        }

      private:
        template <typename F, typename... Ts>
        constexpr decltype(auto) impl(decltype(True), F&& f, Ts&&... ts) const {
          return fused::apply(fused::get<0>(std::forward<F>(f)), std::forward<Ts>(ts)...);
        }
        template <typename F, typename... Ts>
        constexpr decltype(auto) impl(decltype(False), F&& f, Ts&&... ts) const {
          return fused::apply(fused::get<0>(std::forward<F>(f)),
                              operator()(fused::tail(std::forward<F>(f)), std::forward<Ts>(ts)...));
        }
      };
    }

    constexpr auto const compose =
        fused::make_conditional(detail::compose_tuple_{}, detail::compose_args_{});
  }
}
}
