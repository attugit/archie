#pragma once

#include <archie/meta.hpp>
#include <archie/to_type_list.hpp>

namespace archie::fused
{
  namespace detail
  {
    struct to_type_list_ {
      template <template <typename...> typename F, typename... T>
      constexpr decltype(auto) operator()(F<T...> const&) const noexcept
      {
        return fused::type_list<T...>;
      }
    };
  }

  static constexpr auto const& to_type_list = meta::instance<detail::to_type_list_>();
}
