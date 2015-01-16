#pragma once

#include <type_traits>

namespace archie {
namespace utils {
  namespace meta {

    template <typename Condition, typename Tp, typename Up>
    using if_t = typename std::conditional<Condition::value, Tp, Up>::type;
  }
}
}
