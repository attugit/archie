#pragma once

#include <archie/meta/static_constexpr_storage.hpp>
#include <archie/meta/comparable.hpp>

namespace archie::meta
{
  template <typename... T>
  struct type_list : comparable<type_list<T...>> {
  };
}

namespace archie::fused
{
  template <typename... T>
  static constexpr auto const& type_list = meta::instance<meta::type_list<T...>>();
}
