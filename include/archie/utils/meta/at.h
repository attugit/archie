#ifndef ARCHIE_UTILS_META_AT_H_INCLUDED
#define ARCHIE_UTILS_META_AT_H_INCLUDED

#include <utility>
#include <archie/utils/meta/ignore.h>
#include <archie/utils/meta/returns.h>
#include <archie/utils/meta/number.h>

namespace archie {
namespace utils {
  namespace meta {
    template <std::size_t n, typename = std::make_index_sequence<n>>
    struct at;

    template <std::size_t n, std::size_t... other>
    struct at<n, std::index_sequence<other...>> {
    private:
      template <typename Tp>
      static constexpr Tp skip(eat<number<other>>..., Tp&&, ...) noexcept;

    public:
      constexpr at() noexcept = default;
      template <typename... Ts>
      constexpr decltype(auto) operator()(Ts&&... ts) noexcept {
        return *([](eat<number<other>>..., auto&& x, ...) -> decltype(x) {
          return std::forward<decltype(x)>(x);
        }(&std::forward<Ts>(ts)...));
      }

      template <typename... Ts>
      using apply = returns<decltype(skip(std::declval<Ts>()...))>;
    };

    template <std::size_t n, typename... Ts>
    using at_t = typename at<n>::template apply<Ts...>::type;
  }
}
}

#endif
