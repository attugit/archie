#pragma once

#include <utility>
#include <archie/utils/fused/tuple.h>
#include <archie/utils/meta/indexable.h>
#include <archie/utils/meta/boolean.h>
#include <archie/utils/traits/is_fused_tuple.h>
#include <config.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct apply_ {
        template <typename F, typename Tp, typename... Us>
        decltype(auto) operator()(F&& f, Tp&& tp, Us&&... us) const {
          return impl(meta::boolean < traits::is_fused_tuple<
                                          std::remove_reference_t<Tp>>::value &&
                          (sizeof...(Us) == 0) > {},
                      std::forward<F>(f), std::forward<Tp>(tp),
                      std::forward<Us>(us)...);
        }

      private:
        template <typename F, typename... Ts>
        decltype(auto) impl(meta::false_t, F&& f, Ts&&... ts) const {
          return std::forward<F>(f)(std::forward<Ts>(ts)...);
        }
        template <std::size_t... ids>
        struct apply_impl {
          template <typename F, typename Tp>
          constexpr decltype(auto) operator()(F&& f, Tp&& t) const {
            return std::forward<F>(f)(fused::get<ids>(std::forward<Tp>(t))...);
          }
        };
        template <typename F, typename Tp>
        decltype(auto) impl(meta::true_t, F&& f, Tp&& tp) const {
          return meta::indexable_t<
              apply_impl,
              fused::tuple_size<std::remove_reference_t<Tp>>::value>{}(
              std::forward<F>(f), std::forward<Tp>(tp));
        }
      };
    }
    constexpr detail::apply_ apply{};
  }
}
}
