#pragma once

#include <archie/meta/eval.h>

namespace archie {
namespace meta {
  template <typename Tp>
  struct identity {
    using type = Tp;
  };

  template <typename Tp>
  using identity_t = eval<identity<Tp>>;
}
}
