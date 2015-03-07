#pragma once
#include <config.h>

#if defined(USE_ARCHIE_TUPLE)
#include <archie/utils/fused/tuple_archie.h>
#elif defined(USE_STD_TUPLE)
#include <archie/utils/fused/tuple_std.h>
#endif

namespace archie {
namespace utils {
  namespace fused {
    constexpr auto const make_tuple = detail::make_tuple_{};
    constexpr auto const tie = detail::tie_{};
  }
}
}