#pragma once

#include <archie/utils/meta/type_list.h>
#include <archie/utils/meta/static_constexpr_storage.h>

namespace archie {
namespace utils {
  namespace fused {
    template <typename... T>
    static constexpr auto const& type_list = meta::instance<meta::type_list<T...>>();
  }
}
}
