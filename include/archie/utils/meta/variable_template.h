#pragma once

#include <config.h>

#if !defined(HAS_VARIABLE_TEMPLATES)
namespace archie {
namespace utils {
  namespace meta {
    template <typename Tp>
    struct variable_template {
      static constexpr Tp value{};
    };
    template <typename Tp>
    constexpr Tp variable_template<Tp>::value;
  }
}
}
#endif
