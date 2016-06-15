#pragma once

#include <utility>
#include <archie/meta/base.hpp>
#include <archie/meta/at.hpp>
#include <archie/index_of.hpp>

namespace archie::meta
{
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
    using find = decltype(fused::index_of<Up>(fused::type_list<Ts...>));

    template <typename Up>
    using index_of = find<Up>;

    template <typename Up>
    using contains = boolean<find<Up>::value != size::value>;
  };
}
