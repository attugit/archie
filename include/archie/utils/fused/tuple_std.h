#pragma once
#include <config.h>

#if defined(USE_STD_TUPLE)
#include <tuple>
namespace archie {
namespace utils {
  namespace fused {
    using std::tuple;
    using std::get;
    using std::tuple_size;
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
    constexpr auto const make_tuple = detail::make_tuple_{};
    constexpr auto const tie = detail::tie_{};
  }
}
}
#endif