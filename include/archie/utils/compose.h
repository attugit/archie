#ifndef ARCHIE_UTILS_META_COMPOSE_H_INCLUDED
#define ARCHIE_UTILS_META_COMPOSE_H_INCLUDED

namespace archie {
namespace utils {
  namespace meta {

    template <template <typename...> class...>
    struct compose;

    template <template <typename...> class F>
    struct compose<F> {
      template <typename... Xs>
      using apply = typename F<Xs...>::type;
    };

    template <template <typename...> class F,
              template <typename...> class... Gs>
    struct compose<F, Gs...> {
      template <typename... Xs>
      using apply =
          typename F<typename compose<Gs...>::template apply<Xs...>>::type;
    };
  }
}
}

#endif
