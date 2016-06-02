#pragma once

#include <utility>
#include <archie/fused/tuple.hpp>
#include <archie/fused/apply.hpp>
#include <archie/fused/tail.hpp>
#include <archie/boolean.hpp>
#include <archie/fused/static_if.hpp>
#include <archie/traits/is_fused_tuple.hpp>

namespace archie {
namespace fused {
  namespace detail {
    struct compose_ {
      template <typename F, typename... Ts>
      constexpr decltype(auto) operator()(F&& f, Ts&&... ts) const
      {
        int workaround_for_gcc_bug_71095 = 0;
        return fused::static_if(traits::is_fused_tuple<std::decay_t<F>>{})(
            [this](auto&& tpl, auto&&... xs) -> decltype(auto) {
              int workaround_for_gcc_bug_71095_ = 0;
              return fused::static_if(
                  meta::boolean<(tuple_size(type_tag<std::decay_t<decltype(tpl)>>{}) == 1)>{})(
                  [workaround_for_gcc_bug_71095_](auto&& t, auto&&... args) -> decltype(auto) {
                    return fused::apply(fused::get<0>(std::forward<decltype(t)>(t)),
                                        std::forward<decltype(args)>(args)...);
                  },
                  [this](auto&& t, auto&&... args) -> decltype(auto) {
                    return fused::apply(fused::get<0>(std::forward<decltype(t)>(t)),
                                        this->operator()(fused::tail(std::forward<decltype(t)>(t)),
                                                         std::forward<decltype(args)>(args)...));
                  })(std::forward<decltype(tpl)>(tpl), std::forward<decltype(xs)>(xs)...);
            },
            [workaround_for_gcc_bug_71095](auto&& func, auto&&... args) -> decltype(auto) {
              return fused::apply(std::forward<decltype(func)>(func),
                                  std::forward<decltype(args)>(args)...);
            })(std::forward<F>(f), std::forward<Ts>(ts)...);
      }
    };
  }

  static constexpr auto const& compose = meta::instance<detail::compose_>();
}
}
