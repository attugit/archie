#pragma once

#include <iterator>

namespace archie {
inline namespace models {
  struct Iterable {
    template <typename Tp>
    auto requires(Tp&& f) -> decltype(std::begin(f), std::end(f));
  };
}
}
