#pragma once

#include <archie/utils/meta/returns.h>
#include <archie/utils/meta/type_list.h>

namespace archie {
namespace utils {
  namespace meta {
    template <typename>
    struct as_type_list;

    template <template <typename...> class C, typename... Ts>
    struct as_type_list<C<Ts...>> : returns<type_list<Ts...>> {};

    template <typename Tp>
    using as_type_list_t = typename as_type_list<Tp>::type;
  }
}
}
