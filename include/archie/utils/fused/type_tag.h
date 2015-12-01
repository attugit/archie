#pragma once

#include <archie/utils/meta/identity.h>
#include <archie/utils/meta/static_constexpr_storage.h>
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
    static constexpr auto const& make_tag = meta::instance<detail::make_tag_>();
    template <typename T>
    static constexpr auto const& id = meta::instance<type_tag<T>>();
  }
}
}
