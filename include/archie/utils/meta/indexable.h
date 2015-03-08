#pragma once

#include <utility>
#include <archie/utils/meta/eval.h>
#include <archie/utils/meta/returns.h>

namespace archie {
namespace utils {
  namespace meta {
    template <template <std::size_t...> class,
              std::size_t n,
              typename = std::make_index_sequence<n>>
    struct indexable;

    template <template <std::size_t...> class F, std::size_t n, std::size_t... ids>
    struct indexable<F, n, std::index_sequence<ids...>> : returns<F<ids...>> {};

    template <template <std::size_t...> class F, std::size_t n>
    using indexable_t = eval<indexable<F, n>>;
  }
}
}
