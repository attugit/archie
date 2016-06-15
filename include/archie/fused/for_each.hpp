#pragma once

#include <utility>
#include <archie/ignore.hpp>
#include <archie/meta/static_constexpr_storage.hpp>

namespace archie::fused
{
  namespace detail
  {
    struct for_each_ {
      template <typename Func, typename... Args>
      decltype(auto) operator()(Func&& func, Args&&... args) const
      {
        meta::ignore{(std::forward<Func>(func)(std::forward<Args>(args)), 0)...};
        return std::forward<Func>(func);
      }
    };
  }

  static constexpr auto const& for_each = meta::instance<detail::for_each_>();
}
