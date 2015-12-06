#pragma once

namespace archie {
namespace containers {
  namespace detail {
    struct size_ {
      template <typename Tp>
      constexpr decltype(auto) operator()(Tp const& x) const noexcept {
        return x.size();
      }
    };
  }
  constexpr auto const size = detail::size_{};
}
}
