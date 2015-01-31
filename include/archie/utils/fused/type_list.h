#pragma once

#include <archie/utils/meta/type_list.h>

namespace archie {
namespace utils {
  namespace fused {
#if defined(__clang__)
    template <typename... Ts>
    constexpr meta::type_list<Ts...> type_list{};
#endif
  }
}
}
