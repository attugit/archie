#pragma once

#include <utility>
#include <archie/utils/meta/boolean.h>
#include <archie/utils/meta/well_formed.h>

namespace archie {
namespace utils {
  namespace meta {
    template <typename, typename = well_formed_t<>>
    struct model_of : boolean<false> {};

    template <typename Concept, typename... Ts>
    struct model_of<Concept(Ts...),
                    well_formed_t<decltype(std::declval<Concept>().requires(
                        std::declval<Ts>()...))>> : boolean<true> {};
  }
}
}
