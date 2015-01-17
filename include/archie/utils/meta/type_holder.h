#pragma once

namespace archie {
namespace utils {
  namespace meta {

    template <typename Tp>
    struct type_holder {
      using type = Tp;
    };
  }
}
}
