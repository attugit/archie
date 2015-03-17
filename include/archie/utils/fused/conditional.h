#pragma once

#include <utility>
#include <type_traits>
#include <archie/utils/models.h>
#include <archie/utils/traits/model_of.h>
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
        constexpr conditional_() = default;
        template <typename Up>
        constexpr explicit conditional_(Up&& u)
            : F(std::forward<Up>(u)) {}
        template <typename... Ts,
                  typename = meta::requires<traits::model_of<models::Callable(F, Ts&&...)>>>
        constexpr auto operator()(
            Ts&&... xs) const -> decltype(std::declval<F const&>()(std::forward<Ts>(xs)...)) {
          return F::operator()(std::forward<Ts>(xs)...);
        }
      };

      template <typename F1, typename F2>
      struct conditional_<F1, F2> : private F1, private F2 {
        constexpr conditional_() = default;
        template <typename Up, typename... Vs>
        constexpr explicit conditional_(Up&& u, Vs&&... vs)
            : F1(std::forward<Up>(u)), F2(std::forward<Vs>(vs)...) {}
        template <typename... Ts,
                  typename = meta::requires<traits::model_of<models::Callable(F1, Ts&&...)>>>
        constexpr auto operator()(
            Ts&&... xs) const -> decltype(std::declval<F1 const&>()(std::forward<Ts>(xs)...)) {
          return F1::operator()(std::forward<Ts>(xs)...);
        }
        template <typename... Ts,
                  typename = meta::requires_none<traits::model_of<models::Callable(F1, Ts&&...)>>>
        constexpr auto operator()(
            Ts&&... xs) const -> decltype(std::declval<F2 const&>()(std::forward<Ts>(xs)...)) {
          return F2::operator()(std::forward<Ts>(xs)...);
        }
      };

      template <typename F1, typename F2, typename... Fs>
      struct conditional_<F1, F2, Fs...> : conditional_<F1, conditional_<F2, Fs...>> {
        using base_t = conditional_<F1, conditional_<F2, Fs...>>;
        constexpr conditional_() = default;
        template <typename... Us>
        constexpr conditional_(Us&&... us)
            : base_t(std::forward<Us>(us)...) {}
      };

      struct make_conditional_ {
        template <typename... Ts>
        constexpr auto operator()(Ts const&... ts) const {
          return conditional_<Ts...>{ts...};
        }
      };
    }
    template <typename... Fs>
    using conditional_t = detail::conditional_<Fs...>;

#if defined(HAS_VARIABLE_TEMPLATES)
    template <typename... Fs>
    constexpr auto const conditional = conditional_t<Fs...>{};
#else
    template <typename... Fs>
    struct conditional : meta::variable_template<conditional_t<Fs...>> {};
#endif

    constexpr auto const make_conditional = detail::make_conditional_{};
  }
}
}
