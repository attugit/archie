#ifndef ARCHIE_UTILS_VOIDER_H_INCLUDED
#define ARCHIE_UTILS_VOIDER_H_INCLUDED

#include <archie/utils/apply.h>

namespace archie {
namespace utils {

  namespace detail {
    template <typename...>
    struct Voider {
      using type = void;
    };
  }

  template <typename... Ts>
  using void_t = meta::apply_t<detail::Voider, Ts...>;
}
}

#endif
