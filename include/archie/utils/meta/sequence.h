#ifndef ARCHIE_UTILS_META_SEQUENCE_H_INCLUDED
#define ARCHIE_UTILS_META_SEQUENCE_H_INCLUDED

#include <archie/utils/meta/number.h>
#include <archie/utils/meta/boolean.h>
#include <archie/utils/meta/returns.h>
#include <archie/utils/meta/at.h>
#include <archie/utils/meta/inherit_all.h>
#include <archie/utils/meta/otherwise.h>
#include <utility>

namespace archie {
namespace utils {
  namespace meta {

    template <typename...>
    struct sequence;

    namespace detail {

      template <typename Tp>
      struct no_convert {};

      template <std::size_t I, std::size_t T, typename...>
      struct element;

      template <std::size_t I, std::size_t T>
      struct element<I, T> {
        number<T> match(otherwise) const;
      };

      template <std::size_t I, std::size_t T, typename Tp, typename... Ts>
      struct element<I, T, Tp, Ts...> : element<I + 1, T, Ts...> {
        using element<I + 1, T, Ts...>::match;
        number<I> match(no_convert<Tp>) const;
      };

      template <typename Tp, std::size_t N>
      struct element_index {};

      template <std::size_t n, typename = std::make_index_sequence<n>>
      struct index_table;

      template <std::size_t n, std::size_t... idx>
      struct index_table<n, std::index_sequence<idx...>> {
        template <typename... Ts>
        using type = inherit_all<element_index<Ts, idx>...>;
      };

      template <typename... Ts>
      using build_index_table =
          typename index_table<sizeof...(Ts)>::template type<Ts...>;

      template <typename Tp, std::size_t N>
      constexpr number<N> index_of(element_index<Tp, N>) noexcept;

      template <typename...>
      struct is_distinct;

      template <typename Tp, typename... Ts>
      struct is_distinct<Tp, Ts...> {
        using type = typename std::conditional<
            sequence<Ts...>::template contains<Tp>::value, std::false_type,
            typename is_distinct<Ts...>::type>::type;
      };

      template <typename Tp>
      struct is_distinct<Tp> {
        using type = std::true_type;
      };

      template <>
      struct is_distinct<> {
        using type = std::true_type;
      };
    }

    template <typename... Ts>
    struct sequence {
      using size = meta::number<sizeof...(Ts)>;

      template <template <typename...> class Func>
      using apply = typename Func<Ts...>::type;

      template <template <typename> class Func>
      using transform = meta::returns<sequence<typename Func<Ts>::type...>>;

      template <template <typename> class Func>
      using transform_t = typename transform<Func>::type;

      template <typename... Us>
      using append = sequence<Ts..., Us...>;

      template <std::size_t I>
      using at = typename meta::at<I>::template apply<Ts...>;

      template <std::size_t I>
      using at_t = meta::at_t<I, Ts...>;

      template <typename Up>
      using index_of = decltype(detail::index_of<Up>(
          std::declval<detail::build_index_table<Ts...>>()));

      template <typename Up>
      using find = decltype(detail::element<0, size::value, Ts...>{}.match(
          detail::no_convert<Up>{}));

      template <typename Up>
      using contains = boolean<find<Up>::value != size::value>;

      using is_distinct = typename detail::is_distinct<Ts...>::type;
    };
  }
}
}

#endif
