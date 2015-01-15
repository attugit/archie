#pragma once

#include <archie/utils/meta/type_list.h>
#include <archie/utils/meta/index_of.h>
#include <archie/utils/meta/apply.h>

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
  }
}
}
