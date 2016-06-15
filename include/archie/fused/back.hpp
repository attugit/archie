#pragma once

#include <utility>
#include <archie/meta.hpp>
#include <archie/ignore.hpp>

namespace archie::fused
{
  namespace detail
  {
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
