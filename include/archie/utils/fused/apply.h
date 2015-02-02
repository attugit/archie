#pragma once

#include <utility>
#include <archie/utils/fused/tuple.h>

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
      };
    }
    constexpr detail::apply_ apply{};
  }
}
}
