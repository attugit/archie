#pragma once

#include <archie/meta/boolean.hpp>
#include <archie/meta/static_constexpr_storage.hpp>

namespace archie {
namespace fused {
  // FIXME: ODR
  static constexpr auto const True = meta::boolean<true>{};
  static constexpr auto const False = meta::boolean<false>{};
}
}
