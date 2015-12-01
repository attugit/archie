#pragma once

#include <archie/utils/meta/boolean.h>
#include <archie/utils/meta/static_constexpr_storage.h>

namespace archie {
namespace utils {
  namespace fused {
    // FIXME: ODR
    static constexpr auto const True = meta::boolean<true>{};
    static constexpr auto const False = meta::boolean<false>{};
  }
}
}
