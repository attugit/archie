#pragma once

#include <archie/meta/number.hpp>
#include <archie/meta/static_constexpr_storage.hpp>

namespace archie {
namespace fused {
  template <std::size_t N>
  static constexpr auto const& number = meta::instance<meta::number<N>>();
}
}
