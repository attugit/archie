#pragma once

#include <utility>
#include <archie/utils/meta/ignore.h>
#include <archie/utils/meta/number.h>
#include <archie/utils/meta/indexable.h>
#include <archie/utils/fused/nth.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {

      struct order_forward {};
      struct order_backward {};

      template <std::size_t... ids>
      struct for_each_order {
        template <std::size_t n, typename... Ts>
        constexpr decltype(auto) get_n(meta::number<n>, Ts&&... args) const noexcept {
          return VARTEMPL(fused::nth, n)(std::forward<Ts>(args)...);
        }
        template <typename F, typename... Ts>
        decltype(auto) go(order_forward, F&& f, Ts&&... args) const {
          meta::ignore{(f(get_n(meta::number<ids>{}, std::forward<Ts>(args)...)), 0)...};
          return f;
        }
        template <typename F, typename... Ts>
        decltype(auto) go(order_backward, F&& f, Ts&&... args) const {
          meta::ignore{
              (f(get_n(meta::number<sizeof...(ids)-ids - 1>{}, std::forward<Ts>(args)...)), 0)...};
          return f;
        }
        template <typename Order, typename F, typename... Ts>
        decltype(auto) operator()(Order, F&& f, Ts&&... args) const {
          return go(Order{}, std::forward<F>(f), std::forward<Ts>(args)...);
        }
      };
    }
    template <typename F, typename... Ts>
    decltype(auto) for_each_forward(F&& f, Ts&&... args) {
      return meta::indexable_t<detail::for_each_order, sizeof...(Ts)>{}(
          detail::order_forward{}, std::forward<F>(f), std::forward<Ts>(args)...);
    }
    template <typename F, typename... Ts>
    decltype(auto) for_each_backward(F&& f, Ts&&... args) {
      return meta::indexable_t<detail::for_each_order, sizeof...(Ts)>{}(
          detail::order_backward{}, std::forward<F>(f), std::forward<Ts>(args)...);
    }
  }
}
}
