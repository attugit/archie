#pragma once

namespace archie {
namespace utils {
  namespace meta {

    struct ignore final {
      template <typename... Ts>
      ignore(Ts&&...) {}
    };

    template <typename>
    using eat = ignore;
  }
}
}
