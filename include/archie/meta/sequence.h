#pragma once

#include <archie/meta/eval.h>
#include <archie/meta/number.h>
#include <archie/meta/boolean.h>
#include <archie/meta/at.h>
#include <archie/meta/index_of.h>
#include <utility>

namespace archie {
namespace meta {
  template <typename... Ts>
  struct sequence {
    using size = meta::number<sizeof...(Ts)>;

    template <template <typename...> class Func>
    using apply = eval<Func<Ts...>>;

    template <template <typename> class Func>
    using transform = meta::returns<sequence<eval<Func<Ts>>...>>;

    template <template <typename> class Func>
    using transform_t = eval<transform<Func>>;

    template <typename... Us>
    using append = sequence<Ts..., Us...>;

    template <std::size_t I>
    using at = meta::at<I, Ts...>;

    template <std::size_t I>
    using at_t = meta::at_t<I, Ts...>;

    template <typename Up>
    using find = decltype(index_of<Up>(type_list<Ts...>{}));

    template <typename Up>
    using index_of = find<Up>;

    template <typename Up>
    using contains = boolean<find<Up>::value != size::value>;
  };
}
}