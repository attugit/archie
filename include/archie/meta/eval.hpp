#pragma once

namespace archie::meta
{
  template <typename Tp>
  using eval = typename Tp::type;
}
