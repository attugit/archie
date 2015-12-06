#pragma once

#if defined(USE_STD_TUPLE)
#include <tuple>
namespace archie {
namespace fused {
  using std::tuple;
  using std::get;
  using std::tuple_element;
  namespace detail {
    struct make_tuple_ {
      template <typename... Ts>
      constexpr decltype(auto) operator()(Ts&&... ts) const {
        return std::make_tuple(std::forward<Ts>(ts)...);
      }
    };
    struct tie_ {
      template <typename... Ts>
      constexpr decltype(auto) operator()(Ts&&... ts) const {
        return std::tie(std::forward<Ts>(ts)...);
      }
    };
  }
}
}
#endif
