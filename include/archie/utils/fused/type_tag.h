#pragma once

#include <archie/utils/meta/type_holder.h>
#include <config.h>
#include <utility>

namespace archie {
namespace utils {
  namespace fused {

    template <typename Tp>
    struct type_tag : meta::type_holder<Tp> {
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
      struct make_tag {
        template <typename Tp>
        constexpr decltype(auto) operator()(Tp&&) const noexcept {
          return type_tag<std::remove_reference_t<Tp>>{};
        }
      };
    }
    constexpr detail::make_tag make_tag{};
#if defined(HAS_VARIABLE_TEMPLATES)
    template <typename Tp>
    constexpr type_tag<Tp> construct{};
#endif
  }
}
}
