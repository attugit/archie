#pragma once

#include <utility>
#include <config.h>
#include <archie/utils/models/callable.h>
#include <archie/utils/meta/model_of.h>
#include <archie/utils/meta/requires.h>
#include <archie/utils/meta/variable_template.h>

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
        constexpr auto operator()(Ts&&... xs) const
            -> decltype(std::declval<F>()(std::forward<Ts>(xs)...)) {
          return F{}(std::forward<Ts>(xs)...);
        }
      };

      template <typename F1, typename F2>
      struct conditional<F1, F2> {
        template <typename... Ts, typename = meta::requires<meta::model_of<
                                      models::Callable(F1, Ts&&...)>>>
        constexpr auto operator()(Ts&&... xs) const
            -> decltype(std::declval<F1>()(std::forward<Ts>(xs)...)) {
          return F1()(std::forward<Ts>(xs)...);
        }
        template <typename... Ts, typename = meta::requires_none<meta::model_of<
                                      models::Callable(F1, Ts&&...)>>>
        constexpr auto operator()(Ts&&... xs) const
            -> decltype(std::declval<F2>()(std::forward<Ts>(xs)...)) {
          return F2()(std::forward<Ts>(xs)...);
        }
      };

      template <typename F1, typename F2, typename... Fs>
      struct conditional<F1, F2, Fs...>
          : conditional<F1, conditional<F2, Fs...>> {};
    }
    template <typename... Fs>
    using conditional_t = detail::conditional<Fs...>;

#if defined(HAS_VARIABLE_TEMPLATES)
    template <typename... Fs>
    constexpr auto conditional = conditional_t<Fs...>{};
#else
    template <typename... Fs>
    struct conditional : meta::variable_template<conditional_t<Fs...>> {};
#endif
  }
}
}
