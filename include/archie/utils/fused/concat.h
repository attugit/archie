#pragma once

#include <archie/utils/fused/tuple.h>
#include <archie/utils/traits/is_fused_tuple.h>
#include <archie/utils/meta/requires.h>
#include <archie/utils/fused/boolean.h>
#include <archie/utils/meta/as_type_list.h>
#include <archie/utils/fused/conditional.h>

namespace archie {
namespace utils {
  namespace fused {
    namespace detail {
      struct concat_tuples_ {
        template <
            typename Tp,
            typename Up,
            typename = meta::requires_all<traits::is_fused_tuple<std::remove_reference_t<Tp>>,
                                          traits::is_fused_tuple<std::remove_reference_t<Up>>>>
        constexpr decltype(auto) operator()(Tp&& tp, Up&& up) const {
          return impl(std::forward<Tp>(tp), meta::as_type_list_t<std::remove_reference_t<Tp>>{},
                      std::forward<Up>(up), meta::as_type_list_t<std::remove_reference_t<Up>>{});
        }

      private:
        template <typename Tp, typename Up, typename... Ts, typename... Us>
        constexpr decltype(auto) impl(Tp&& tp,
                                      meta::type_list<Ts...>,
                                      Up&& up,
                                      meta::type_list<Us...>) const {
          return fused::make_tuple(fused::get<Ts>(std::forward<Tp>(tp))...,
                                   fused::get<Us>(std::forward<Up>(up))...);
        }
      };

      struct append_tuple_ {
        template <typename Tp,
                  typename... Us,
                  typename = meta::requires<traits::is_fused_tuple<std::remove_reference_t<Tp>>>>
        constexpr decltype(auto) operator()(Tp&& tp, Us&&... us) const {
          return impl(std::forward<Tp>(tp), meta::as_type_list_t<std::remove_reference_t<Tp>>{},
                      std::forward<Us>(us)...);
        }

      private:
        template <typename Tp, typename... Ts, typename... Us>
        constexpr decltype(auto) impl(Tp&& tp, meta::type_list<Ts...>, Us&&... us) const {
          return fused::make_tuple(fused::get<Ts>(std::forward<Tp>(tp))...,
                                   std::forward<Us>(us)...);
        }
      };
    }
    constexpr auto const concat =
        fused::make_conditional(detail::concat_tuples_{}, detail::append_tuple_{});
  }
}
}
