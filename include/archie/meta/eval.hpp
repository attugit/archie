#pragma once

namespace archie {
namespace meta {
  template <typename Tp>
  using eval = typename Tp::type;
}
}
