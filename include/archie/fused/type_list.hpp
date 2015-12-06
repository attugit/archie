#pragma once

#include <archie/meta/type_list.hpp>
#include <archie/meta/static_constexpr_storage.hpp>

namespace archie {
namespace fused {
  template <typename... T>
  static constexpr auto const& type_list = meta::instance<meta::type_list<T...>>();
}
}
