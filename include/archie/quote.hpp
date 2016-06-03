#pragma once

#include <archie/meta/static_constexpr_storage.hpp>
#include <archie/meta/comparable.hpp>

namespace archie::meta
{
  template <typename T>
  struct quote : comparable<quote<T>> {
  };
}

namespace archie::fused
{
  template <typename T>
  static constexpr auto const& quote = meta::instance<meta::quote<T>>();
}
