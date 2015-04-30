#pragma once

namespace archie {
namespace utils {
  namespace containers {
    namespace detail {
      struct capacity_ {
        template <typename Tp>
        constexpr decltype(auto) operator()(Tp const& x) const noexcept {
          return x.capacity();
        }
      };
    }
    constexpr auto const capacity = detail::capacity_{};
  }
}
}
