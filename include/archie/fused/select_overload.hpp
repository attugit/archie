#pragma once

#include <archie/meta/ignore.hpp>

namespace archie {
namespace fused {
  template <unsigned N>
  struct choice : choice<N + 1> {
  };

  template <>
  struct choice<10> {
  };

  template <>
  struct choice<0> final : choice<1> {
  };

  constexpr choice<0> select_overload{};
  using otherwise = meta::ignore;
}
}
