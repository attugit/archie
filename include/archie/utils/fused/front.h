#pragma once

#include <archie/utils/fused/placeholder.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct front_ : fused::placeholder<0> {};
    }
    constexpr detail::front_ front{};
  }
}
}
