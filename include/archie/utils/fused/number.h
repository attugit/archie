#pragma once

#include <archie/utils/meta/number.h>
#include <archie/utils/meta/static_constexpr_storage.h>

namespace archie {
namespace utils {
  namespace fused {
    template <std::size_t N>
    static constexpr auto const& number = meta::instance<meta::number<N>>();
  }
}
}
