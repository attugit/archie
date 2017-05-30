#pragma once

#include <utility>
#include <archie/fused/fold.hpp>

namespace archie::fused
{
  constexpr auto const compose = fused::make_fold([](auto s, auto f) {
    return [s, f](auto&&... args) -> decltype(auto) {
      return s(f(std::forward<decltype(args)>(args)...));
    };
  });
}
