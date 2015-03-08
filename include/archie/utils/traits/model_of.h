#pragma once

#include <utility>
#include <archie/utils/fused/boolean.h>
#include <archie/utils/meta/well_formed.h>

namespace archie {
namespace utils {
  namespace traits {
    template <typename, typename = meta::well_formed_t<>>
    struct model_of : decltype(fused::False) {};

    template <typename Concept, typename... Ts>
    struct model_of<
        Concept(Ts...),
        meta::well_formed_t<decltype(std::declval<Concept>().requires(std::declval<Ts>()...))>>
        : decltype(fused::True) {};
  }
}
}
