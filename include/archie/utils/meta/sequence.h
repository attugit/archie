#ifndef ARCHIE_UTILS_META_SEQUENCE_H_INCLUDED
#define ARCHIE_UTILS_META_SEQUENCE_H_INCLUDED

#include <archie/utils/meta/number.h>
#include <archie/utils/meta/boolean.h>
#include <archie/utils/meta/at.h>
#include <archie/utils/meta/index_of.h>
#include <utility>

namespace archie {
namespace utils {
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
      using at = typename meta::at<I>::template apply<Ts...>;

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
}

#endif
