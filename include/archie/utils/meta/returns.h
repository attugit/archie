#pragma once

#include <archie/utils/meta/identity.h>

namespace archie {
namespace utils {
  namespace meta {
    template <typename Tp>
    using returns = identity<Tp>;
  }
}
}
