#pragma once

#include <archie/utils/meta/eval.h>
#include <archie/utils/meta/type_list.h>
#include <archie/utils/meta/returns.h>
#include <archie/utils/meta/functional.h>
#include <archie/utils/meta/logic.h>

namespace archie {
namespace utils {
  namespace meta {
    template <typename, typename>
    struct append;

    template <typename... Ts, typename... Us>
    struct append<type_list<Ts...>, type_list<Us...>> : returns<type_list<Ts..., Us...>> {};

    template <typename Tp, typename... Us>
    struct append<type_list<Us...>, Tp> : returns<type_list<Us..., Tp>> {};

    template <typename Tp, typename Up>
    using append_t = eval<append<Tp, Up>>;

    template <template <typename> class, typename...>
    struct append_if;

    template <template <typename> class F, typename Tp, typename... Us>
    struct append_if<F, type_list<Us...>, Tp>
        : returns<if_t<eval<F<Tp>>, append_t<type_list<Us...>, Tp>, type_list<Us...>>> {};

    template <template <typename> class F, typename... Ts>
    using append_if_t = eval<append_if<F, Ts...>>;

    template <template <typename> class, typename...>
    struct append_if_not;

    template <template <typename> class F, typename Tp, typename... Us>
    struct append_if_not<F, type_list<Us...>, Tp>
        : returns<if_t<opposite_t<eval<F<Tp>>>, append_t<type_list<Us...>, Tp>, type_list<Us...>>> {
    };

    template <template <typename> class F, typename... Ts>
    using append_if_not_t = eval<append_if_not<F, Ts...>>;
  }
}
}
