#pragma once

#include <archie/meta/identity.hpp>

namespace archie {
namespace meta {
  template <typename Tp>
  using returns = identity<Tp>;
}
}
