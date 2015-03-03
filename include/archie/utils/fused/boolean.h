#pragma once

#include <archie/utils/meta/boolean.h>

namespace archie {
namespace utils {
  namespace fused {
    constexpr auto const True = meta::boolean<true>{};
    constexpr auto const False = meta::boolean<false>{};
  }
}
}
