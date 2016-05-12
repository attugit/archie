#pragma once

#include <archie/meta/index_of.hpp>
#include <archie/fused/type_list.hpp>
#include <archie/meta/static_constexpr_storage.hpp>
#include <utility>

namespace archie {
namespace fused {
  namespace detail {
    template <typename Tp>
    struct index_of_ {
      template <typename... Us>
      constexpr decltype(auto) operator()(Us&&...) const noexcept
      {
        return meta::index_of<Tp>(fused::type_list<Us...>);
      }
      template <typename... Us>
      constexpr decltype(auto) operator()(meta::type_list<Us...> x) const noexcept
      {
        return meta::index_of<Tp>(x);
      }
    };
  }
  template <typename T>
  static constexpr auto const& index_of = meta::instance<detail::index_of_<T>>();
}
}
