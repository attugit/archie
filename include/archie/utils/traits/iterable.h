#pragma once

#include <iterator>

namespace archie {
namespace utils {
  inline namespace models {
    struct Iterable {
      template <typename Tp>
      auto requires(Tp&& f) -> decltype(std::begin(f), std::end(f));
    };
  }
}
}
