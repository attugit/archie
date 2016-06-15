#pragma once

#include <utility>
#include <archie/meta/base.hpp>
#include <archie/meta/listed.hpp>

namespace archie::meta
{
  template <template <typename...> class, std::size_t n, typename = std::make_index_sequence<n>>
  struct indexable;

  template <template <typename...> class F, std::size_t n, std::size_t... ids>
  struct indexable<F, n, std::index_sequence<ids...>> : returns<F<meta::number<ids>...>> {
  };

  template <template <typename...> class F, std::size_t n>
  using indexable_t = eval<indexable<F, n>>;
}
