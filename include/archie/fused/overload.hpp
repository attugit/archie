#pragma once

#include <utility>
#include <type_traits>

namespace archie::fused
{
  template <typename... F>
  struct overload : F... {
    constexpr overload() = default;
    constexpr overload(F... f) : F(std::move(f))... {}
    using F::operator()...;
  };
}
