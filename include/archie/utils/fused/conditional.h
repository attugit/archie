#pragma once

#include <utility>
#include <type_traits>
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
      struct conditional_;

      template <typename F>
      struct conditional_<F> : private F {
        template <typename... Ts, typename = meta::requires<meta::model_of<
                                      models::Callable(F, Ts&&...)>>>
        constexpr auto operator()(Ts&&... xs) const
            -> decltype(std::declval<F const&>()(std::forward<Ts>(xs)...)) {
          return F::operator()(std::forward<Ts>(xs)...);
        }
      };

      template <typename F1, typename F2>
      struct conditional_<F1, F2> : private F1, private F2 {
        template <typename... Ts, typename = meta::requires<meta::model_of<
                                      models::Callable(F1, Ts&&...)>>>
        constexpr auto operator()(Ts&&... xs) const
            -> decltype(std::declval<F1 const&>()(std::forward<Ts>(xs)...)) {
          return F1::operator()(std::forward<Ts>(xs)...);
        }
        template <typename... Ts, typename = meta::requires_none<meta::model_of<
                                      models::Callable(F1, Ts&&...)>>>
        constexpr auto operator()(Ts&&... xs) const
            -> decltype(std::declval<F2 const&>()(std::forward<Ts>(xs)...)) {
          return F2::operator()(std::forward<Ts>(xs)...);
        }
      };

      template <typename F1, typename F2, typename... Fs>
      struct conditional_<F1, F2, Fs...>
          : conditional_<F1, conditional_<F2, Fs...>> {};

      struct make_conditional_ {
        template <typename... Ts>
        constexpr auto operator()(Ts const&...) const {
          return conditional_<Ts...>{};
        }
      };
    }
    template <typename... Fs>
    using conditional_t = detail::conditional_<Fs...>;

#if defined(HAS_VARIABLE_TEMPLATES)
    template <typename... Fs>
    constexpr auto conditional = conditional_t<Fs...>{};
#else
    template <typename... Fs>
    struct conditional : meta::variable_template<conditional_t<Fs...>> {};
#endif

    constexpr auto make_conditional = detail::make_conditional_{};
  }
}
}
