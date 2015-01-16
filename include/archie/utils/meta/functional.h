#pragma once

#include <type_traits>
#include <archie/utils/meta/boolean.h>

namespace archie {
namespace utils {
  namespace meta {

    template <typename Condition, typename Tp, typename Up>
    using if_t = typename std::conditional<Condition::value, Tp, Up>::type;

    template <typename Lhs, typename Rhs>
    using less_t = if_t<boolean<(Lhs::value < Rhs::value)>, true_t, false_t>;

    template <typename Lhs, typename Rhs>
    using greater_t = less_t<Rhs, Lhs>;
  }
}
}
