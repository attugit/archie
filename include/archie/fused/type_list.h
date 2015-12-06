#pragma once

#include <archie/meta/type_list.h>
#include <archie/meta/static_constexpr_storage.h>

namespace archie {
namespace fused {
  template <typename... T>
  static constexpr auto const& type_list = meta::instance<meta::type_list<T...>>();
}
}
