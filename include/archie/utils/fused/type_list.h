#pragma once

#include <config.h>
#include <archie/utils/meta/type_list.h>
#include <archie/utils/meta/variable_template.h>

namespace archie {
namespace utils {
  namespace fused {
#if defined(HAS_VARIABLE_TEMPLATES)
    template <typename... Ts>
    constexpr meta::type_list<Ts...> type_list{};
#else
    template <typename... Ts>
    struct type_list : meta::variable_template<meta::type_list<Ts...>> {};
#endif
  }
}
}
