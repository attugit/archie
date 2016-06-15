#pragma once

#include <archie/meta/eval.hpp>

namespace archie::meta
{
  template <typename Tp>
  struct identity {
    using type = Tp;
  };

  template <typename Tp>
  using identity_t = eval<identity<Tp>>;
}
