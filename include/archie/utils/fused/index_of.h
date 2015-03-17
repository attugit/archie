#pragma once

#include <archie/utils/meta/index_of.h>
#include <archie/utils/fused/type_list.h>
#include <archie/utils/meta/variable_template.h>
#include <utility>


namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      template <typename Tp>
      struct index_of_ {
        template <typename... Us>
        constexpr decltype(auto) operator()(Us&&...) const noexcept {
#if defined(HAS_VARIABLE_TEMPLATES)
          return meta::index_of<Tp>(type_list<Us...>);
#else
          return meta::index_of<Tp>(type_list<Us...>::value);
#endif
        }
        template <typename... Us>
        constexpr decltype(auto) operator()(meta::type_list<Us...> x) const noexcept {
          return meta::index_of<Tp>(x);
        }
      };
    }
#if defined(HAS_VARIABLE_TEMPLATES)
    template <typename Tp>
    constexpr detail::index_of_<Tp> index_of{};
#else
    template <typename Tp>
    struct index_of : meta::variable_template<detail::index_of_<Tp>> {};
#endif
  }
}
}
