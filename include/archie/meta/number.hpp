#pragma once

#include <utility>

namespace archie {
namespace meta {
  template <std::size_t N>
  using number = std::integral_constant<std::size_t, N>;
}
}
