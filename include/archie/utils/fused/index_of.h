#pragma once

#include <utility>
#include <archie/utils/meta/index_of.h>
#include <archie/utils/fused/type_list.h>

namespace archie {
namespace utils {
  namespace fused {
#if defined(__clang__)
    namespace detail {
      template <typename Tp>
      struct index_of {
        template <typename... Us>
        constexpr decltype(auto) operator()(Us&&...) const noexcept {
          return meta::index_of<Tp>(type_list<Us...>);
        }
        template <typename... Us>
        constexpr decltype(auto) operator()(meta::type_list<Us...> x) const
            noexcept {
          return meta::index_of<Tp>(x);
        }
      };
    }
    template <typename Tp>
    constexpr detail::index_of<Tp> index_of{};
#endif
  }
}
}
