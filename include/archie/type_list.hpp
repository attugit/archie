#pragma once

#include <archie/meta.hpp>

namespace archie::meta
{
  template <typename... T>
  struct type_list : comparable<type_list<T...>> {
  };
}

namespace archie::fused
{
  template <typename... T>
  static constexpr auto const& type_list = meta::instance<meta::type_list<T...>>();
}

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
