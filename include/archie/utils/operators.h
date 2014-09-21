#ifndef ARCHIE_UTILS_OPERATORS_H_INCLUDED
#define ARCHIE_UTILS_OPERATORS_H_INCLUDED

#include <archie/utils/requires.h>

namespace archie {
namespace utils {
  namespace detail {

    template <typename Tp, typename Up>
    struct is_less_comparable_helper {
      template <typename Tp1, typename Up1,
                typename = decltype(std::declval<Tp1>() < std::declval<Up1>())>
      static std::true_type test(int);

      template <typename, typename>
      static std::false_type test(...);

      using type = decltype(test<Tp, Up>(0));
    };

    template <typename Tp, typename Up>
    struct is_equality_comparable_helper {
      template <typename Tp1, typename Up1,
                typename = decltype(std::declval<Tp1>() == std::declval<Up1>())>
      static std::true_type test(int);

      template <typename, typename>
      static std::false_type test(...);

      using type = decltype(test<Tp, Up>(0));
    };
  }

  template <typename...>
  struct is_less_comparable;

  template <typename Tp, typename Up>
  struct is_less_comparable<Tp, Up>
      : detail::is_less_comparable_helper<Tp, Up>::type {};

  template <typename Tp>
  struct is_less_comparable<Tp>
      : detail::is_less_comparable_helper<Tp, Tp>::type {};

  template <typename...>
  struct is_equality_comparable;

  template <typename Tp, typename Up>
  struct is_equality_comparable<Tp, Up>
      : detail::is_equality_comparable_helper<Tp, Up>::type {};

  template <typename Tp>
  struct is_equality_comparable<Tp>
      : detail::is_equality_comparable_helper<Tp, Tp>::type {};

  template <typename T>
  struct Operators {
    friend bool operator!=(T const& lhs, T const& rhs) {
      return (lhs < rhs) || (lhs > rhs);
    }
    friend bool operator==(T const& lhs, T const& rhs) { return !(lhs != rhs); }
    friend bool operator>(T const& lhs, T const& rhs) { return rhs < lhs; }
    friend bool operator>=(T const& lhs, T const& rhs) { return !(lhs < rhs); }
    friend bool operator<=(T const& lhs, T const& rhs) { return !(lhs > rhs); }
  };
}
}

#endif
