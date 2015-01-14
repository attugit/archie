#pragma once

#include <archie/utils/meta/type_list.h>
#include <archie/utils/meta/returns.h>

namespace archie {
namespace utils {
  namespace meta {

    template <typename, typename>
    struct append;

    template <typename... Ts, typename... Us>
    struct append<type_list<Ts...>, type_list<Us...>>
        : returns<type_list<Ts..., Us...>> {};

    template <typename Tp, typename... Us>
    struct append<type_list<Us...>, Tp> : returns<type_list<Us..., Tp>> {};

    template <typename Tp, typename Up>
    using append_t = typename append<Tp, Up>::type;
  }
}
}
