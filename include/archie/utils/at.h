#ifndef ARCHIE_UTILS_META_AT_H_INCLUDED
#define ARCHIE_UTILS_META_AT_H_INCLUDED

#include <utility>
#include <archie/utils/eat.h>
#include <archie/utils/returns.h>
#include <archie/utils/number.h>

namespace archie {
namespace utils {
  namespace meta {
    template <std::size_t n, typename = std::make_index_sequence<n>>
    struct at;

    template <std::size_t n, std::size_t... ignore>
    struct at<n, std::index_sequence<ignore...>> {
    private:
      template <typename... Ts>
      struct skip {
        template <typename Up>
        static constexpr Up match(eat<Ts>..., Up u, ...) noexcept {
          return u;
        }

        template <typename... Us>
        using apply = returns<decltype(match(std::declval<Us>()...))>;
      };

    public:
      template <typename... Ts>
      using apply = typename skip<Number<ignore>...>::template apply<Ts...>;

      template <typename... Ts>
      static constexpr decltype(auto) match(Ts&&... args) noexcept {
        return *skip<Number<ignore>...>::match(&std::forward<Ts>(args)...);
      }
    };

    template <std::size_t n, typename... Ts>
    using at_t = typename at<n>::template apply<Ts...>::type;
  }
}
}

#endif
