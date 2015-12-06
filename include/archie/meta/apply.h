#pragma once

#include <archie/meta/eval.h>
#include <archie/meta/type_list.h>
#include <archie/meta/returns.h>

namespace archie {
namespace meta {
  template <template <typename...> class F, typename... Args>
  struct apply : returns<eval<F<Args...>>> {};

  template <template <typename...> class F, typename... Args>
  struct apply<F, type_list<Args...>> : apply<F, Args...> {};

  template <template <typename...> class F, typename... Args>
  using apply_t = eval<apply<F, Args...>>;
}
}
