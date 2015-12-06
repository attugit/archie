#pragma once

#include <archie/meta/identity.h>

namespace archie {
namespace meta {
  template <typename Tp>
  using returns = identity<Tp>;
}
}
