#pragma once

#include <archie/utils/meta/apply.h>
#include <archie/utils/meta/ignore.h>
#include <archie/utils/meta/returns.h>

namespace archie {
namespace utils {
  namespace meta {
    template <typename...>
    struct well_formed : returns<ignore> {};

    template <typename... Ts>
    using well_formed_t = apply_t<well_formed, Ts...>;
  }
}
}
