#pragma once

#include <type_traits>
#include <archie/meta/eval.hpp>
#include <archie/meta/logic.hpp>
#include <archie/boolean.hpp>
#include <archie/quote.hpp>
#include <archie/ignore.hpp>
#include <archie/type_list.hpp>
#include <archie/meta/well_formed.hpp>

namespace archie::meta
{
  namespace detail
  {
    template <std::size_t, typename T>
    struct indexed_type : quote<T> {
    };

    template <typename... T>
    struct inherit : T... {
    };

    template <typename, typename, typename = well_formed_t<>>
    struct is_distinct : decltype(fused::False) {
    };

    template <std::size_t... I, typename... T>
    struct is_distinct<std::index_sequence<I...>,
                       type_list<T...>,
                       well_formed_t<void(decltype(static_cast<quote<T>>(
                           std::declval<inherit<indexed_type<I, T>...>>()))...)>>
        : decltype(fused::True) {
    };
  }

  template <typename... T>
  using is_distinct = eval<detail::is_distinct<std::index_sequence_for<T...>, type_list<T...>>>;
}
