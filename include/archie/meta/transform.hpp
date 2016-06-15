#pragma once

#include <archie/meta.hpp>
#include <archie/type_list.hpp>

namespace archie::meta
{
  template <template <typename> class F, typename... Xs>
  struct transform {
    using type = type_list<typename F<Xs>::type...>;
  };

  template <template <typename> class F, typename... Xs>
  struct transform<F, type_list<Xs...>> : transform<F, Xs...> {
  };

  template <template <typename> class F, typename... Xs>
  using transform_t = eval<transform<F, Xs...>>;
}
