#pragma once

#include <archie/utils/meta/eval.h>

namespace archie {
namespace utils {
  namespace meta {
    template <typename Tp>
    struct identity {
      using type = Tp;
    };

    template <typename Tp>
    using identity_t = eval<identity<Tp>>;
  }
}
}
