#pragma once

#include <config.h>

#if !defined(HAS_VARIABLE_TEMPLATES)
namespace archie {
namespace utils {
  namespace meta {
    template <typename Tp>
    struct variable_template {
      static constexpr decltype(Tp{}) const value = Tp{};
    };
    template <typename Tp>
    constexpr decltype(Tp{}) const variable_template<Tp>::value;
  }
}
}
#endif
