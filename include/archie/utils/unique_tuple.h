#ifndef ARCHIE_UTILS_UNIQUE_TUPLE_H_INCLUDED
#define ARCHIE_UTILS_UNIQUE_TUPLE_H_INCLUDED

#include <archie/utils/meta/requires.h>

namespace archie {
namespace utils {
  namespace detail {

    template <typename Tp, typename Up, typename... Tail>
    struct Contains {
      using type =
          typename std::conditional<std::is_same<Tp, Up>::value, std::true_type,
                                    typename Contains<Tp, Tail...>::type>::type;
    };

    template <typename Tp, typename Up>
    struct Contains<Tp, Up> {
      using type = std::is_same<Tp, Up>;
    };

    template <typename Tp, typename... Tail>
    struct IsUnique {
      using type = typename std::conditional<
          detail::Contains<Tp, Tail...>::type::value, std::false_type,
          typename IsUnique<Tail...>::type>::type;
    };

    template <typename Tp>
    struct IsUnique<Tp> {
      using type = std::true_type;
    };
  }

  template <typename Tp, typename... Types>
  struct Contains : detail::Contains<Tp, Types...>::type {};

  template <typename... Types>
  struct IsUnique : detail::IsUnique<Types...>::type {};

  template <typename... Types>
  struct UniqueTuple : std::tuple<Types...> {
    static_assert(IsUnique<Types...>::value, "");
    using BaseType = std::tuple<Types...>;
    using BaseType::BaseType;
  };
}
}

#endif
