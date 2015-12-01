#pragma once

#include <archie/utils/meta/ignore.h>
#include <archie/utils/meta/static_constexpr_storage.h>

namespace archie {
namespace utils {
  namespace fused {
    static constexpr auto const& ignore = meta::instance<meta::ignore>();
  }
}
}
