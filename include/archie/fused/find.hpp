#pragma once

#include <archie/meta/find.hpp>
#include <archie/meta/static_constexpr_storage.hpp>
#include <archie/fused/nth.hpp>

namespace archie {
namespace fused {
  namespace detail {
    template <typename Tp>
    struct find_ {
      template <typename... Us>
      constexpr decltype(auto) operator()(Us&&... us) const noexcept {
        return fused::nth<meta::find_t<Tp, Us...>::value>(std::forward<Us>(us)...);
      }
    };
    template <template <typename> class F>
    struct find_if_ {
      template <typename... Us>
      constexpr decltype(auto) operator()(Us&&... us) const noexcept {
        return fused::nth<meta::find_if_t<F, Us...>::value>(std::forward<Us>(us)...);
      }
    };
  }
  template <typename T>
  static constexpr auto const& find = meta::instance<detail::find_<T>>();
  template <template <typename> class F>
  static constexpr auto const& find_if = meta::instance<detail::find_if_<F>>();
}
}
