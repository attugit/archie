#pragma once

#include <archie/meta/apply.hpp>

namespace archie {
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
