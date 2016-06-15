#pragma once

#include <archie/meta.hpp>
#include <archie/index_of.hpp>
#include <archie/meta/find.hpp>
#include <archie/fused/nth.hpp>

namespace archie::fused
{
  namespace detail
  {
    template <typename Tp>
    struct find_ {
      template <typename... Us>
      constexpr decltype(auto) operator()(Us&&... us) const noexcept
      {
        return fused::nth<index_of<Tp>(type_list<Us...>)>(std::forward<Us>(us)...);
      }
    };
    template <template <typename> class F>
    struct find_if_ {
      template <typename... Us>
      constexpr decltype(auto) operator()(Us&&... us) const noexcept
      {
        return fused::nth<meta::find_if_t<F, Us...>::value>(std::forward<Us>(us)...);
      }
    };
  }
  template <typename T>
  static constexpr auto const& find = meta::instance<detail::find_<T>>();
  template <template <typename> class F>
  static constexpr auto const& find_if = meta::instance<detail::find_if_<F>>();
}
