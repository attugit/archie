#pragma once

#include <archie/utils/meta/type_list.h>
#include <archie/utils/meta/index_of.h>
#include <archie/utils/meta/apply.h>
#include <archie/utils/meta/boolean.h>

namespace archie {
namespace utils {
  namespace meta {
    namespace detail {
      template <typename Tp, typename... Us>
      using find = decltype(index_of<Tp>(type_list<Us...>{}));
    }

    template <typename Tp, typename... Us>
    struct find : detail::find<Tp, Us...> {};

    template <typename Tp, typename... Us>
    struct find<Tp, type_list<Us...>> : find<Tp, Us...> {};

    template <typename Tp, typename... Us>
    using find_t = apply_t<find, Tp, Us...>;

    template <template <typename> class F, typename... Ts>
    struct find_if : find<true_t, boolean<F<Ts>::value>...> {};

    template <template <typename> class F, typename... Ts>
    struct find_if<F, type_list<Ts...>> : find_if<F, Ts...> {};

    template <template <typename> class F, typename... Ts>
    using find_if_t = typename find_if<F, Ts...>::type;
  }
}
}
