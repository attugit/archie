#pragma once

#include <utility>
#include <archie/utils/fused/tuple.h>
#include <archie/utils/meta/indexable.h>
#include <config.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct apply_ {
        template <typename F, typename Tp, typename... Us>
        decltype(auto) operator()(F&& f, Tp&& x, Us&&... y) const
            noexcept(noexcept(std::declval<F>()(std::declval<Tp>(),
                                                std::declval<Us>()...))) {
          return std::forward<F>(f)(std::forward<Tp>(x),
                                    std::forward<Us>(y)...);
        }
#if defined(USE_ARCHIE_TUPLE)
        template <typename F, typename... Ts>
        decltype(auto) operator()(F&& f, fused::tuple<Ts...>& x) const {
          return x.apply(std::forward<F>(f));
        }
        template <typename F, typename... Ts>
        decltype(auto) operator()(F&& f, fused::tuple<Ts...> const& x) const {
          return x.apply(std::forward<F>(f));
        }
        template <typename F, typename... Ts>
        decltype(auto) operator()(F&& f, fused::tuple<Ts...>&& x) const {
          return x.apply(std::forward<F>(f));
        }
#elif defined(USE_STD_TUPLE)
        template <std::size_t... ids>
        struct apply_impl {
          template <typename F, typename Tp>
          constexpr decltype(auto) operator()(F&& f, Tp&& t) const {
            return std::forward<F>(f)(fused::get<ids>(std::forward<Tp>(t))...);
          }
        };
        template <typename F, typename... Ts>
        decltype(auto) operator()(F&& f, fused::tuple<Ts...>& x) const {
          return meta::indexable_t<apply_impl, sizeof...(Ts)>{}(
              std::forward<F>(f), x);
        }
        template <typename F, typename... Ts>
        decltype(auto) operator()(F&& f, fused::tuple<Ts...> const& x) const {
          return meta::indexable_t<apply_impl, sizeof...(Ts)>{}(
              std::forward<F>(f), x);
        }
        template <typename F, typename... Ts>
        decltype(auto) operator()(F&& f, fused::tuple<Ts...>&& x) const {
          return meta::indexable_t<apply_impl, sizeof...(Ts)>{}(
              std::forward<F>(f), x);
        }
#endif
      };
    }
    constexpr detail::apply_ apply{};
  }
}
}
