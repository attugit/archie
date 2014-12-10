#ifndef ARCHIE_UTILS_FUSED_BACK_H_INCLUDED
#define ARCHIE_UTILS_FUSED_BACK_H_INCLUDED

#include <utility>
#include <archie/utils/meta/eat.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      template <typename... Ts>
      struct back_impl {
        template <typename Up>
        static constexpr decltype(auto) match(meta::eat<Ts>..., Up&& u,
                                              ...) noexcept {
          return std::forward<Up>(u);
        }
      };
    }

    template <typename Tp, typename... Ts>
    constexpr decltype(auto) back(Tp&& t, Ts&&... ts) noexcept {
      return detail::back_impl<Ts...>::match(std::forward<Tp>(t),
                                             std::forward<Ts>(ts)...);
    }
  }
}
}

#endif
