#pragma once

#include <utility>
#include <archie/utils/models/callable.h>
#include <archie/utils/meta/model_of.h>
#include <archie/utils/meta/requires.h>
#include <config.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      template <typename, typename...>
      struct conditional;

      template <typename F>
      struct conditional<F> {
        template <typename... Ts, typename = meta::requires<meta::model_of<
                                      models::Callable(F, Ts&&...)>>>
        auto operator()(Ts&&... xs)
            -> decltype(std::declval<F>()(std::forward<Ts>(xs)...)) {
          return F{}(std::forward<Ts>(xs)...);
        }
      };

      template <typename F1, typename F2>
      struct conditional<F1, F2> {
        template <typename... Ts, typename = meta::requires<meta::model_of<
                                      models::Callable(F1, Ts&&...)>>>
        auto operator()(Ts&&... xs)
            -> decltype(std::declval<F1>()(std::forward<Ts>(xs)...)) {
          return F1()(std::forward<Ts>(xs)...);
        }
        template <typename... Ts, typename = meta::requires_none<meta::model_of<
                                      models::Callable(F1, Ts&&...)>>>
        auto operator()(Ts&&... xs)
            -> decltype(std::declval<F2>()(std::forward<Ts>(xs)...)) {
          return F2()(std::forward<Ts>(xs)...);
        }
      };

      template <typename F1, typename F2, typename... Fs>
      struct conditional<F1, F2, Fs...>
          : conditional<F1, conditional<F2, Fs...>> {};
    }
    template <typename... Fs>
    using conditional = detail::conditional<Fs...>;
  }
}
}
