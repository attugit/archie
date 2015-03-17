#pragma once

#include <archie/utils/meta/type_list.h>
#include <archie/utils/meta/variable_template.h>

namespace archie {
namespace utils {
  namespace fused {
#if defined(HAS_VARIABLE_TEMPLATES)
    template <typename... Ts>
    constexpr auto const type_list = meta::type_list<Ts...>{};
#else
    template <typename... Ts>
    struct type_list : meta::variable_template<meta::type_list<Ts...>> {};
#endif
  }
}
}
