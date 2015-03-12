#pragma once

#include <config.h>
#include <archie/utils/meta/identity.h>
#include <archie/utils/meta/variable_template.h>
#include <archie/utils/fused/boolean.h>
#include <utility>

namespace archie {
namespace utils {
  namespace fused {
    template <typename Tp>
    struct type_tag : meta::identity<Tp> {
      template <typename... Us>
      constexpr decltype(auto) construct(Us&&... us) const {
        return Tp{std::forward<Us>(us)...};
      }
      template <typename... Us>
      constexpr decltype(auto) operator()(Us&&... us) const {
        return construct(std::forward<Us>(us)...);
      }
    };
    template <typename Tp, typename Up>
    constexpr bool operator==(type_tag<Tp> const&, type_tag<Up> const&) {
      return std::is_same<Tp, Up>::value;
    }
    template <typename Tp, typename Up>
    constexpr bool operator!=(type_tag<Tp> const&, type_tag<Up> const&) {
      return !std::is_same<Tp, Up>::value;
    }

    namespace detail {
      struct make_tag_ {
        template <typename Tp>
        constexpr decltype(auto) operator()(Tp&&) const noexcept {
          return type_tag<std::remove_reference_t<Tp>>{};
        }
      };
    }
    constexpr auto make_tag = detail::make_tag_{};
#if defined(HAS_VARIABLE_TEMPLATES)
    template <typename Tp>
    constexpr auto const id = type_tag<Tp>{};
#else
    template <typename Tp>
    struct id : meta::variable_template<type_tag<Tp>> {};
#endif
  }
}
}
