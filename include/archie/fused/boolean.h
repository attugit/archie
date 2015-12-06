#pragma once

#include <archie/meta/boolean.h>
#include <archie/meta/static_constexpr_storage.h>

namespace archie {
namespace fused {
  // FIXME: ODR
  static constexpr auto const True = meta::boolean<true>{};
  static constexpr auto const False = meta::boolean<false>{};
}
}
