#pragma once

#include <archie/utils/meta/number.h>
#include <archie/utils/meta/variable_template.h>

namespace archie {
namespace utils {
  namespace fused {
#if !defined(HAS_VARIABLE_TEMPLATES)
    template <std::size_t N>
    struct number : meta::variable_template<meta::number<N>> {};
#else
    template <std::size_t N>
    constexpr meta::number<N> number{};
#endif
  }
}
}
