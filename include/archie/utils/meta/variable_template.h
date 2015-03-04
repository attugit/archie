#pragma once

#include <config.h>

#if !defined(HAS_VARIABLE_TEMPLATES)
namespace archie {
namespace utils {
  namespace meta {
    template <typename Tp>
    struct variable_template {
      static constexpr auto const value = Tp{};
    };
    template <typename Tp>
    constexpr Tp const variable_template<Tp>::value;
  }
}
}
#endif
