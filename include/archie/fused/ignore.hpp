#pragma once

#include <archie/meta/ignore.hpp>
#include <archie/meta/static_constexpr_storage.hpp>

namespace archie {
namespace fused {
  static constexpr auto const& ignore = meta::instance<meta::ignore>();
}
}
