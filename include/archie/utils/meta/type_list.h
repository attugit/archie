#ifndef ARCHIE_UTILS_TYPE_LIST_H_INCLUDED
#define ARCHIE_UTILS_TYPE_LIST_H_INCLUDED

#include <archie/utils/meta/number.h>
#include <archie/utils/meta/boolean.h>
#include <archie/utils/meta/returns.h>
#include <archie/utils/meta/at.h>
#include <archie/utils/meta/inherit_all.h>
#include <utility>

namespace archie {
namespace utils {
  namespace meta {
    namespace detail {

      struct otherwise {
        otherwise(...) {}
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

      template <typename Tp, std::size_t N, std::size_t A>
      constexpr number<N> index_of(element_index<Tp, N>, number<A>) noexcept;

      template <typename, std::size_t A>
      constexpr number<A> index_of(otherwise, number<A>) noexcept;
    }

    template <typename... Ts>
    struct type_list {
      using size = meta::number<sizeof...(Ts)>;

      template <template <typename...> class Func>
      using apply = typename Func<Ts...>::type;

      template <template <typename> class Func>
      using transform = meta::returns<type_list<typename Func<Ts>::type...>>;

      template <template <typename> class Func>
      using transform_t = typename transform<Func>::type;

      template <typename... Us>
      using append = type_list<Ts..., Us...>;

      template <std::size_t I>
      using at = typename meta::at<I>::template apply<Ts...>;

      template <std::size_t I>
      using at_t = meta::at_t<I, Ts...>;

      template <typename Up>
      using index_of = decltype(detail::index_of<Up>(
          std::declval<detail::build_index_table<Ts...>>(), size{}));

      template <typename Up>
      using contains = boolean<index_of<Up>::value != size::value>;
    };
  }
}
}

#endif
