#pragma once

#include <archie/meta/number.h>
#include <archie/meta/static_constexpr_storage.h>

namespace archie {
namespace fused {
  template <std::size_t N>
  static constexpr auto const& number = meta::instance<meta::number<N>>();
}
}
