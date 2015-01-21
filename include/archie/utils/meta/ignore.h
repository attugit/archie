#pragma once

namespace archie {
namespace utils {
  namespace meta {

    struct ignore final {
      template <typename... Ts>
      constexpr ignore(Ts&&...) noexcept {}
    };

    template <typename>
    using eat = ignore;
  }
}
}
