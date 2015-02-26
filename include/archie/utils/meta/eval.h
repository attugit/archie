#pragma once

namespace archie {
namespace utils {
  namespace meta {
    template <typename Tp>
    using eval = typename Tp::type;
  }
}
}
