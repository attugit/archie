#pragma once

namespace archie::meta
{
  template <template <typename...> class...>
  struct compose;

  template <template <typename...> class F>
  struct compose<F> {
    template <typename... Xs>
    using apply = F<Xs...>;
  };

  template <template <typename...> class F, template <typename...> class... Gs>
  struct compose<F, Gs...> {
    template <typename... Xs>
    using apply = F<typename compose<Gs...>::template apply<Xs...>::type>;
  };
}
