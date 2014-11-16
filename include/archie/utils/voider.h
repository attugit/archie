#ifndef ARCHIE_UTILS_VOIDER_H_INCLUDED
#define ARCHIE_UTILS_VOIDER_H_INCLUDED

#include <archie/utils/apply.h>

namespace archie {
namespace utils {

  namespace detail {
    template <typename...>
    struct MakeVoid {
      using type = void;
    };
  }

  template <typename... Ts>
  using Voider = Apply<detail::MakeVoid, Ts...>;
}
}

#endif
