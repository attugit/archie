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
      template <typename F1, typename F2>
      struct basic_conditional {
        template <class... Ts, typename = meta::requires<meta::model_of<
                                   models::Callable(F1, Ts&&...)>>>
        auto operator()(Ts&&... xs) -> decltype(F1()(std::forward<Ts>(xs)...)) {
          return F1()(std::forward<Ts>(xs)...);
        }
        template <class... Ts, typename = meta::requires_none<meta::model_of<
                                   models::Callable(F1, Ts&&...)>>>
        auto operator()(Ts&&... xs) -> decltype(F2()(std::forward<Ts>(xs)...)) {
          return F2()(std::forward<Ts>(xs)...);
        }
      };

      template <typename F, typename... Fs>
      struct conditional : basic_conditional<F, conditional<Fs...>> {};

      template <typename F>
      struct conditional<F> : F {};
    }
    template <typename... Fs>
    using conditional = detail::conditional<Fs...>;
  }
}
}
