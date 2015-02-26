#pragma once

#include <archie/utils/meta/boolean.h>

namespace archie {
namespace utils {
  namespace fused {
    constexpr meta::boolean<true> True{};
    constexpr meta::boolean<false> False{};
  }
}
}
