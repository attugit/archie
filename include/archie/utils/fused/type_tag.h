#pragma once

#include <config.h>
#include <archie/utils/meta/identity.h>
#include <archie/utils/meta/variable_template.h>
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

    namespace detail {
      struct make_tag_ {
        template <typename Tp>
        constexpr decltype(auto) operator()(Tp&&) const noexcept {
          return type_tag<std::remove_reference_t<Tp>>{};
        }
      };
    }
    constexpr detail::make_tag_ make_tag{};
#if defined(HAS_VARIABLE_TEMPLATES)
    template <typename Tp>
    constexpr type_tag<Tp> construct{};
#else
    template <typename Tp>
    struct construct : meta::variable_template<type_tag<Tp>> {};
#endif
  }
}
}
