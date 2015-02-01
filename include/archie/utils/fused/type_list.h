#pragma once

#include <archie/utils/meta/type_list.h>
#include <config.h>

namespace archie {
namespace utils {
  namespace fused {
#if defined(HAS_VARIABLE_TEMPLATES)
    template <typename... Ts>
    constexpr meta::type_list<Ts...> type_list{};
#endif
  }
}
}
