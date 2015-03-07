#pragma once

#include <archie/utils/traits/has_member.h>

namespace archie {
namespace utils {
  namespace traits {
    namespace detail {
      template <typename F, typename... Ts>
      using inspect_callable =
          decltype(std::declval<F>()(std::declval<Ts>()...));
    }
    template <typename F, typename... Ts>
    using is_callable =
        has_member<detail::inspect_callable, meta::type_list<F, Ts...>>;
  }
}
}
