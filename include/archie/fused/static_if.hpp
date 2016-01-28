#pragma once

#include <utility>
#include <archie/meta/static_constexpr_storage.hpp>

namespace archie {
namespace fused {
  namespace detail {
    struct static_if_ {
      template <typename Cond, typename IfTrue, typename IfFalse>
      decltype(auto) operator()(Cond const&, IfTrue&& if_true, IfFalse&& if_false) const {
        return select(std::integral_constant<bool, std::decay_t<Cond>::value>{},
                      std::forward<IfTrue>(if_true), std::forward<IfFalse>(if_false));
      }

    private:
      template <typename IfTrue, typename IfFalse>
      decltype(auto) select(std::true_type, IfTrue&& if_true, IfFalse&&) const {
        return std::forward<IfTrue>(if_true);
      }
      template <typename IfTrue, typename IfFalse>
      decltype(auto) select(std::false_type, IfTrue&&, IfFalse&& if_false) const {
        return std::forward<IfFalse>(if_false);
      }
    };
  }

  static constexpr auto const& static_if = meta::instance<detail::static_if_>();
}
}
