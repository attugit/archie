#ifndef ARCHIE_UTILS_VOIDER_H_INCLUDED
#define ARCHIE_UTILS_VOIDER_H_INCLUDED

#include <archie/utils/meta/apply.h>

namespace archie {
namespace utils {
  namespace meta {
    namespace detail {
      template <typename...>
      struct voider {
        using type = void;
      };
    }

    template <typename... Ts>
    using void_t = apply_t<detail::voider, Ts...>;
  }
}
}

#endif
