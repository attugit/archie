#pragma once

#include <utility>
#include <archie/utils/fused/tuple.h>
#include <archie/utils/meta/indexable.h>
#include <archie/utils/meta/requires.h>
#include <archie/utils/fused/conditional.h>
#include <archie/utils/traits/is_fused_tuple.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct apply_args_ {
        template <typename F, typename... Ts>
        decltype(auto) operator()(F&& f, Ts&&... ts) const {
          return std::forward<F>(f)(std::forward<Ts>(ts)...);
        }
      };

      struct apply_tuple_ {
        template <typename F,
                  typename Tp,
                  typename = meta::requires<traits::is_fused_tuple<std::remove_reference_t<Tp>>>>
        constexpr decltype(auto) operator()(F&& f, Tp&& tp) const {
          return meta::indexable_t<impl_, tuple_size(type_tag<Tp>{})>{}(std::forward<F>(f),
                                                                        std::forward<Tp>(tp));
        }

      private:
        template <std::size_t... ids>
        struct impl_ {
          template <typename F, typename Tp>
          constexpr decltype(auto) operator()(F&& f, Tp&& t) const {
            return apply_args_{}(std::forward<F>(f), fused::get<ids>(std::forward<Tp>(t))...);
          }
        };
      };
    }

    static constexpr auto const apply =
        make_conditional(detail::apply_tuple_{}, detail::apply_args_{});
  }
}
}
