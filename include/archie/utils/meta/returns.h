#pragma once

#include <archie/utils/meta/type_holder.h>

namespace archie {
namespace utils {
  namespace meta {

    template <typename Tp>
    using returns = type_holder<Tp>;

    template <typename Tp>
    using eval = typename Tp::type;
  }
}
}
