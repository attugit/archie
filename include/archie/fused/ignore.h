#pragma once

#include <archie/meta/ignore.h>
#include <archie/meta/static_constexpr_storage.h>

namespace archie {
namespace fused {
  static constexpr auto const& ignore = meta::instance<meta::ignore>();
}
}
