#pragma once

#include <utility>
#include <archie/ignore.hpp>
#include <archie/meta/static_constexpr_storage.hpp>

namespace archie {
namespace fused {
  namespace detail {
    struct back_ {
      template <typename Tp, typename... Us>
      constexpr decltype(auto) operator()(Tp&& t, Us&&... us) const noexcept
      {
        return [](meta::eat<Us>..., auto&& x) -> decltype(x) {
          return std::forward<decltype(x)>(x);
        }(std::forward<Tp>(t), std::forward<Us>(us)...);
      }
    };
  }
  static constexpr auto const& back = meta::instance<detail::back_>();
}
}
