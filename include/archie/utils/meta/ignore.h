#pragma once

#include <cstddef>

namespace archie {
namespace utils {
  namespace meta {

    struct ignore final {
      template <typename... Ts>
      constexpr ignore(Ts&&...) noexcept {}
    };

    template <typename>
    using eat = ignore;

    template <std::size_t>
    using eat_n = ignore;
  }
}
}
