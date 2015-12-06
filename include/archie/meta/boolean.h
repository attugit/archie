#pragma once

#include <utility>

namespace archie {
namespace meta {
  template <bool B>
  using boolean = std::integral_constant<bool, B>;

  using true_t = boolean<true>;
  using false_t = boolean<false>;
}
}
