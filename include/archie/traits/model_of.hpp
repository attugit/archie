#pragma once

#include <utility>
#include <archie/boolean.hpp>
#include <archie/meta/well_formed.hpp>

namespace archie::traits
{
  template <typename, typename = meta::well_formed_t<>>
  struct model_of : decltype(fused::False) {
  };

  template <typename Concept, typename... Ts>
  struct model_of<
      Concept(Ts...),
      meta::well_formed_t<decltype(std::declval<Concept>().require(std::declval<Ts>()...))>>
      : decltype(fused::True) {
  };
}
