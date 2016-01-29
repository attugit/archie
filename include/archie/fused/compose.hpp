#pragma once

#include <utility>
#include <archie/fused/tuple.hpp>
#include <archie/fused/apply.hpp>
#include <archie/fused/tail.hpp>
#include <archie/fused/boolean.hpp>
#include <archie/fused/static_if.hpp>
#include <archie/traits/is_fused_tuple.hpp>

namespace archie {
namespace fused {
  namespace detail {
    struct compose_ {
      template <typename F, typename... Ts>
      constexpr decltype(auto) operator()(F&& f, Ts&&... ts) const {
        return fused::static_if(traits::is_fused_tuple<std::decay_t<F>>{})(
            [this](auto&& tpl, auto&&... xs) -> decltype(auto) {
              return fused::static_if(meta::boolean<(tuple_size(type_tag<F>{}) == 1)>{})(
                  [](auto&& t, auto&&... args) -> decltype(auto) {
                    return fused::apply(fused::get<0>(std::forward<decltype(t)>(t)),
                                        std::forward<decltype(args)>(args)...);
                  },
                  [this](auto&& t, auto&&... args) -> decltype(auto) {
                    return fused::apply(fused::get<0>(std::forward<decltype(t)>(t)),
                                        this->operator()(fused::tail(std::forward<decltype(t)>(t)),
                                                         std::forward<decltype(args)>(args)...));
                  })(std::forward<decltype(tpl)>(tpl), std::forward<decltype(xs)>(xs)...);
            },
            [](auto&& func, auto&&... args) -> decltype(auto) {
              return fused::apply(std::forward<decltype(func)>(func),
                                  std::forward<decltype(args)>(args)...);
            })(std::forward<F>(f), std::forward<Ts>(ts)...);
      }
    };
  }

  static constexpr auto const& compose = meta::instance<detail::compose_>();
}
}
