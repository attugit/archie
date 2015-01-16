#pragma once

#include <type_traits>
#include <archie/utils/meta/boolean.h>
#include <archie/utils/meta/number.h>
#include <archie/utils/meta/returns.h>
#include <archie/utils/meta/type_list.h>

namespace archie {
namespace utils {
  namespace meta {

    template <typename Condition, typename Tp, typename Up>
    using if_t = eval<std::conditional<Condition::value, Tp, Up>>;

    template <typename Lhs, typename Rhs>
    using less_t = if_t<boolean<(Lhs::value < Rhs::value)>, true_t, false_t>;

    template <typename Lhs, typename Rhs>
    using greater_t = less_t<Rhs, Lhs>;

    template <typename Tp, typename... Us>
    struct sum : returns<number<Tp::value + eval<sum<Us...>>::value>> {};

    template <typename Tp>
    struct sum<Tp> : returns<number<Tp::value>> {};

    template <typename Tp, typename... Us>
    struct sum<type_list<Tp, Us...>> : sum<Tp, Us...> {};

    template <typename... Ts>
    using sum_t = eval<sum<Ts...>>;

    template <unsigned... N>
    using usum_t = sum_t<number<N>...>;
  }
}
}
