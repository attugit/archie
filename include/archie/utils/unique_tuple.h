#ifndef ARCHIE_UTILS_UNIQUE_TUPLE_H_INCLUDED
#define ARCHIE_UTILS_UNIQUE_TUPLE_H_INCLUDED

#include <archie/utils/requires.h>

namespace archie {
namespace utils {
  namespace detail {

    template <typename, typename...>
    struct Contains;

    template <typename Tp, typename Up>
    struct Contains<Tp, Up> {
      using type = std::is_same<Tp, Up>;
    };

    template <typename Tp, typename Up, typename... Tail>
    struct Contains<Tp, Up, Tail...> {
      using type =
          typename std::conditional<std::is_same<Tp, Up>::value, std::true_type,
                                    typename Contains<Tp, Tail...>::type>::type;
    };

    template <typename...>
    struct IsUnique;

    template <typename Tp>
    struct IsUnique<Tp> {
      using type = std::true_type;
    };

    template <typename Tp, typename... Tail>
    struct IsUnique<Tp, Tail...> {
      using self = typename detail::Contains<Tp, Tail...>::type;
      using rest = typename detail::IsUnique<Tail...>::type;
      using type = typename std::conditional<
          (!self::value) && rest::value, std::true_type, std::false_type>::type;
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
