#ifndef ARCHIE_UTILS_TYPE_LIST_H_INCLUDED
#define ARCHIE_UTILS_TYPE_LIST_H_INCLUDED

#include <archie/utils/number.h>
#include <archie/utils/meta/returns.h>
#include <archie/utils/meta/at.h>
#include <utility>

namespace archie {
namespace utils {

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
  };
}
}

#endif
