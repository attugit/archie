#ifndef ARCHIE_UTILS_TYPE_LIST_H_INCLUDED
#define ARCHIE_UTILS_TYPE_LIST_H_INCLUDED

#include <archie/utils/number.h>
#include <archie/utils/returns.h>
#include <archie/utils/at.h>
#include <utility>

namespace archie {
namespace utils {

  template <typename First, typename... Rest>
  struct get_front {
    using type = First;
  };

  template <template <typename...> class...>
  struct compose;

  template <template <typename...> class F>
  struct compose<F> {
    template <typename... Xs>
    using apply = typename F<Xs...>::type;
  };

  template <template <typename...> class F, template <typename...> class... Gs>
  struct compose<F, Gs...> {
    template <typename... Xs>
    using apply =
        typename F<typename compose<Gs...>::template apply<Xs...>>::type;
  };

  template <typename... Ts>
  struct type_list {
    using size = Number<sizeof...(Ts)>;

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
  namespace meta {
    template <template <typename> class F, typename... Xs>
    using transform = typename type_list<Xs...>::template transform<F>;

    template <template <typename> class F, typename... Xs>
    using transform_t = typename transform<F, Xs...>::type;
  }
}
}

#endif
