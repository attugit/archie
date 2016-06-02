#pragma once

#include <utility>
#include <archie/meta/ignore.hpp>
#include <archie/meta/number.hpp>
#include <archie/type_list.hpp>

namespace archie {
namespace meta {
  namespace detail {
    template <typename Tp>
    struct no_convert {
    };

    template <std::size_t I, std::size_t T, typename...>
    struct element;

    template <std::size_t I, std::size_t T>
    struct element<I, T> {
      number<T> match(ignore) const;
    };

    template <std::size_t I, std::size_t T, typename Tp, typename... Ts>
    struct element<I, T, Tp, Ts...> : element<I + 1, T, Ts...> {
      using element<I + 1, T, Ts...>::match;
      number<I> match(no_convert<Tp>) const;
    };
  }

  template <typename Tp, typename... Ts>
  constexpr decltype(auto) index_of(type_list<Ts...>) noexcept
  {
    using type =
        decltype(detail::element<0, sizeof...(Ts), Ts...>{}.match(detail::no_convert<Tp>{}));
    return type{};
  }

  template <typename Tp, typename... Ts>
  constexpr decltype(auto) type_index(Ts...) noexcept
  {
    return index_of<Tp>(type_list<Ts...>{});
  }
}
}
