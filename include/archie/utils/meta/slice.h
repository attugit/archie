#pragma once

#include <archie/utils/meta/eval.h>
#include <archie/utils/meta/take.h>
#include <archie/utils/meta/tail.h>
#include <archie/utils/meta/type_list.h>

namespace archie {
namespace utils {
  namespace meta {
    template <std::size_t i, std::size_t j, typename... Ts>
    struct slice : returns<tail_t<i, take_t<j, Ts...>>> {};

    template <std::size_t i, std::size_t j, typename... Ts>
    struct slice<i, j, type_list<Ts...>> : slice<i, j, Ts...> {};

    template <std::size_t i, std::size_t j, typename... Ts>
    using slice_t = eval<slice<i, j, Ts...>>;
  }
}
}
