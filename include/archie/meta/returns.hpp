#pragma once

#include <archie/meta/identity.hpp>

namespace archie::meta
{
  template <typename Tp>
  using returns = identity<Tp>;
}
