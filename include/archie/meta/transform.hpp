#pragma once

#include <archie/meta/eval.hpp>
#include <archie/meta/type_list.hpp>

namespace archie {
namespace meta {
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
}
