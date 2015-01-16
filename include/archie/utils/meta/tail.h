#pragma once

#include <utility>
#include <archie/utils/meta/returns.h>
#include <archie/utils/meta/type_list.h>
#include <archie/utils/meta/ignore.h>
#include <archie/utils/meta/number.h>

namespace archie {
namespace utils {
  namespace meta {
    template <std::size_t n, typename = std::make_index_sequence<n>>
    struct tail;

    template <std::size_t n, std::size_t... other>
    struct tail<n, std::index_sequence<other...>> {
    private:
      template <typename... Ts>
      static constexpr type_list<Ts...> slice(eat<number<other>>..., Ts...);

    public:
      template <typename... Ts>
      using apply = returns<decltype(slice(std::declval<Ts>()...))>;
    };

    template <std::size_t n, typename... Ts>
    using tail_t = typename tail<n>::template apply<Ts...>::type;
  }
}
}
